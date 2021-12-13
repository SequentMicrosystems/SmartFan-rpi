
module.exports = function(RED) {
    "use strict";
    var I2C = require("i2c-bus");
    const DEFAULT_HW_ADD = 0x03;
    const I2C_MEM_FAN_POWER = 0; 

    function PowerNode(n) {
        RED.nodes.createNode(this, n);
        this.stack = parseInt(n.stack);
        this.payload = n.payload;
        this.payloadType = n.payloadType;
        var node = this;
        //var buffer = Buffer.alloc(2);
        
        node.port = I2C.openSync( 1 );
        node.on("input", function(msg) {
            var myPayload;
            var stack = node.stack; 
            if (isNaN(stack)) stack = msg.stack;
            stack = parseInt(stack);
            if (this.payloadType == null) {
                myPayload = this.payload;
            } else if (this.payloadType == 'none') {
                myPayload = null;
            } else {
                myPayload = RED.util.evaluateNodeProperty(this.payload, this.payloadType, this,msg);
            }
            if (isNaN(stack+1)) {
                this.status({fill:"red",shape:"ring",text:"Stack level ("+stack+") value is missing or incorrect"});
                return;
            } else if(isNaN(myPayload)){
              this.status({fill:"red",shape:"ring",text:"Payload type must be a number  ("+this.payload+") value is missing or incorrect myPayload: ("+myPayload+")"});
                return;
            }
            else{
                this.status({});
            }

            try {
                var hwAdd = DEFAULT_HW_ADD;
                 if(stack < 0){
                    stack = 0;
                }
                if(stack > 1){
                  stack = 1;
                }
                hwAdd += stack;

                if(myPayload < 0){
                  myPayload = 0;
                }
                if(myPayload > 100){
                  myPayload = 100;
                }
                var intVal = parseInt(myPayload);
                node.port.writeByte(hwAdd, I2C_MEM_FAN_POWER, intVal,  function(err, size, res) {
                    if (err) { 
                        node.error(err, msg);
                    } 
                    else{
                       
                    }
                    });     
                    
            } catch(err) {
                this.error(err,msg);
            }
            
        });

        node.on("close", function() {
            node.port.closeSync();
        });
    }
    RED.nodes.registerType("SFAN Power", PowerNode); 

}