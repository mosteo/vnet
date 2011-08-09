#ifndef vnet_stage_h_
#define vnet_stage_h_

#include <boost/functional/factory.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include "vnet_message.h"
#include "vnet_support.h"

namespace vnet {

// A stage is a pluggable network piece (filter, transport).
// In the sender side, the message travels following the Stage send method.
// In the receiving side, the message travels following the Stage received method.

// In the sender side, however, the message might be addressed to a group of nodes.
// It's after the "transport" stage that messages have a single recipient.
// (The original recipients are however given so a node can know that it's not the only recipient).

    class Stage;
    typedef boost::shared_ptr<Stage> StageRef;

    class Stage {
    public:
        // Plain constructor
        Stage () : upstream_ (StageRef()), downstream_ (StageRef()) {};      
        
        // Constructor with options, for filters and future transports.
        Stage (const boost::program_options::variables_map &vm) : upstream_ (StageRef()), downstream_ (StageRef()) {};      
        
        //  To allow identification of stages by name
        virtual std::string name () const = 0;
        
        virtual void send     (const Message &msg, const Envelope &meta) = 0;
        virtual void received (const Message &msg, const Envelope &meta, const NodeId &receiver) = 0;        
        
        //  These setters are thread-safe
        void set_upstream   (StageRef upstream)   { upstream_  .set (upstream);   };
        void set_downstream (StageRef downstream) { downstream_.set (downstream); };
        
        virtual ~Stage() {};

        //  These accessors are thread-safe
        StageRef upstream  () const { return upstream_  .get (); };
        StageRef downstream() const { return downstream_.get (); };
    
    private:  
        atomic<StageRef> upstream_  ;
        atomic<StageRef> downstream_;
    };

    
    // A filter is intended to receive a message and selectively drop it if necessary
    class Filter : public Stage {
    public:
        virtual void send     (const Message &msg, const Envelope &meta);
        virtual void received (const Message &msg, const Envelope &meta, const NodeId &receiver);
    
    protected:
        virtual bool accept_send    (const Message &msg, const Envelope &meta) const = 0;
        // Override this with the actual test against the message when sending
        
        virtual bool accept_receive (const Message &msg, const Envelope &meta, const NodeId &receiver) const = 0;
        // Override this with the actual test against the message when receiving
    };

    // A transport is a special stage in the sense that it is at the end of a stage chain. 
    // It is intended to actually pass along over a message to a (possibly) remote endpoint.
    class Transport : public Stage {
    };
    
    // Boost::factory appears for the first time in 1.43.0
    class StageFactory {
    public:
        template <typename SomeStage>
        static void register_stage(const std::string name);
        
        static StageRef create(const std::string name, 
                               const boost::program_options::variables_map &vm);
        
    private:
        //  The stage factory.
        //  TODO: make it thread-safe?
        typedef boost::function<Stage * (const boost::program_options::variables_map &)> StageConstructor;
        static std::map<const std::string, StageConstructor> factory_;                    
    };
    
    /////////////////////////////////////////////
    // IMPLEMENTATION OF NON-TRIVIAL TEMPLATES //
    /////////////////////////////////////////////
    
    template <typename SomeStage>
    void vnet::StageFactory::register_stage(const std::string name) 
    { 
        if (factory_.find (name) != factory_.end ())
            throw std::runtime_error (name + ": already registered in factory.");
        else
            factory_[name] = boost::factory<SomeStage*>(); 
    };

}

#endif