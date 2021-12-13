# node-red-contrib-sm-sfan

This is the node-red node to control Sequent Microsystems [Smart Fan Hat](https://sequentmicrosystems.com/collections/all-io-cards/products/raspberry-pi-fan).

## Install

Clone or update the repository, follow the instrutions fron the [first page.](https://github.com/SequentMicrosystems/SmartFan-rpi)

In your node-red user directory, tipicaly ~/.node-red,

```bash
~$ cd ~/.node-red
```

Run the following command:

```bash
~/.node-red$ npm install ~/SmartFan-rpi/node-red-contrib-sm-sfan
```

In order to see the node in the palette and use-it you need to restart node-red. If you run node-red as a service:
 ```bash
 ~$ node-red-stop
 ~$ node-red-start
 ```

## Usage

After install and restart the node-red you will see on the node palete, under Sequent Microsystems category one new node:

### SFAN Power node

This node provides a connection to an Sequent Microsystems Smart Fan blow power.
The card stack level can be set in the node dialog box or dynamically through ```msg.stack``` `.
You can set the power from 0% to 100% thru ```msg.payload``` controling fan speed.

## Important note

This node is using the I2C-bus package from @fivdi, you can visit his work on github [here](https://github.com/fivdi/i2c-bus). 
The inspiration for this node came from @nielsnl68 work with [node-red-contrib-i2c](https://github.com/nielsnl68/node-red-contrib-i2c).Thank both for the great job.
