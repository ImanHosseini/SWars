var Victor = require('victor');
const ipc = require('electron').ipcRenderer;
const canvas = document.getElementById('myCanvas');
const ctx = canvas.getContext('2d');
const image = new Image;

class Ship {
    static vThrottle=  new Victor(3.0,3.0);
    static ROT = 0.0225; 
    static vDT = new Victor(0.2, 0.2);
    static Vm = 50.0;
   
    constructor(pos = new Victor(0.0,0.0), vel = new Victor(0.0,0.0), acc = new Victor(0.0,0.0), rot = 0.0) {
        this.pos = pos;
        this.vel = vel;
        this.acc = acc;
        this.rot = rot;
        this.head = new Victor(0.0, -1.0);
    }

    

    updatePos() {
        this.pos = this.pos.add(this.vel.multiply(Ship.vDT));
    }
    
    updateVel() {
        this.vel = this.vel.add(this.acc.multiply(Ship.vDT));
    }
    updateAcc(throttle) {
        if (throttle) {
            this.acc = this.head.multiply(Ship.vThrottle);
            var v = Math.exp(-this.vel.length()/Ship.Vm);
            this.acc = this.head.multiply(Ship.vThrottle).multiply(new Victor(v,v));
            console.log(this.pos.x + "**" + this.pos.y);
            } else {
            this.acc = new Victor(0.0, 0.0);
        }
    }

}

var ship = new Ship();

var KeyboardHelper = { left: 37, up: 38, right: 39, down: 40 , space: 32};
document.addEventListener('keydown', keyDownHandler, false);
document.addEventListener('keyup', keyUpHandler, false);
var rightPressed = false;
var leftPressed = false;
var upPressed = false;
var downPressed = false;
var spacePressed = false;
function keyDownHandler(event) {
    if (event.keyCode == 39) {
        rightPressed = true;
    }
    else if (event.keyCode == 37) {
        leftPressed = true;
    }
    if (event.keyCode == 40) {
        downPressed = true;
    }
    else if (event.keyCode == 38) {
        upPressed = true;
    }
    else if (event.keyCode == KeyboardHelper.space) {
        spacePressed = true;
    }
    var cntrl = 0;
    cntrl += spacePressed ? 1 : 0;
    cntrl += rightPressed ? 2 : 0;
    cntrl += leftPressed ? 4 : 0;
    //var cnt = new ArrayBuffer(1);
    //var v_cnt = new DataView(cnt);
    //v_cnt.setUint8(0, cntrl);
  //  console.log("CC" + cntrl);
    ipc.send('asynchronous-message', cntrl);
}
function keyUpHandler(event) {
    if (event.keyCode == 39) {
        rightPressed = false;
    }
    else if (event.keyCode == 37) {
        leftPressed = false;
    }
    if (event.keyCode == 40) {
        downPressed = false;
    }
    else if (event.keyCode == 38) {
        upPressed = false;
    } else if (event.keyCode == KeyboardHelper.space) {
        spacePressed = false;
    }
    var cntrl = 0;
    cntrl += spacePressed ? 1 : 0;
    cntrl += rightPressed ? 2 : 0;
    cntrl += leftPressed ? 4 : 0;
    //var cnt = new ArrayBuffer(1);
    //var v_cnt = new DataView(cnt);
    //v_cnt.setUint8(0, cntrl);
    //  console.log("CC" + cntrl);
    ipc.send('asynchronous-message', cntrl);
}
console.log('ligg');
image.src = 'assets/spaceship.png';
// image.onload = drawImg();
// window.requestAnimationFrame(draw);
function drawImg() {
    console.log('drawn');
    ctx.drawImage(image,0,0 ,460/4, 783/4);
}
function draw() {
    ctx.clearRect(0, 0, canvas.clientWidth, canvas.clientHeight);
    console.log('drawn');
    ship.updatePos();
    ship.updateVel();
    if (rightPressed) {
        ship.head.rotate(Ship.ROT);
        ship.rot += Ship.ROT;
    }
    else if (leftPressed) {
        ship.head.rotate(-Ship.ROT);
        ship.rot += -Ship.ROT;
    }
    //if (downPressed) {
    //    ctx.translate(0, 5);
    //}
    //else if (upPressed) {
    //    ctx.translate(0, -5);
    //}
    if (spacePressed) {
        ship.updateAcc(true);
        console.log("acc");
    } else {
        ship.updateAcc(false);
    }
    ctx.save();
    ctx.translate(ship.pos.x,ship.pos.y);
    ctx.rotate(ship.rot);
    ctx.drawImage(image, -460/20, -783/20, 460 / 10, 783 / 10);
    ctx.restore();
    window.requestAnimationFrame(draw);
}

ipc.on('message', (event, message) => {
    console.log(message.length);
    var m = message.length / 12;
    var msg = new ArrayBuffer(message.length);
    var view2 = new DataView(msg);
    var frm = "";
    for (var idx = 0; idx < message.length / 4; idx++) {
        frm += idx + ":" + message[idx].toString() + " | ";
        frm += idx+1 + ":" + message[idx+1].toString() + " | ";
        frm += idx+2 + ":" + message[idx+2].toString() + " | ";
        frm += idx+3 + ":" + message[idx+3].toString() + " | ";
        view2.setUint8(idx*4+3, message[idx*4]);
        view2.setUint8(idx*4+2, message[idx*4+1]);
        view2.setUint8(idx*4+1, message[idx*4+2]);
        view2.setUint8(idx*4, message[idx*4+3]);
    }
    console.log(frm);
 
    ctx.clearRect(0, 0, canvas.clientWidth, canvas.clientHeight);
    for (var i = 0; i < m; i++) {
        var x = view2.getFloat32(12 * i);
        var y = view2.getFloat32(12 * i + 4);
        var t = view2.getFloat32(12 * i + 8);
        console.log("x:" + x + "  y:" + y);
        ctx.save();
        ctx.translate(x, y);
        ctx.rotate(-t);
        ctx.drawImage(image, -460 / 20, -783 / 20, 460 / 10, 783 / 10);
        ctx.restore();
    }
    //var buf2 = new ArrayBuffer(4);
    //// Create a data view of it
   
    //var view2 = new DataView(buf2);
    //// set bytes

    //view2.setUint8(3, message[0]);
    //view2.setUint8(2, message[1]);
    //view2.setUint8(1, message[2]);
    //view2.setUint8(0, message[3]);
    //// Read the bits as a float; note that by doing this, we're implicitly
    //// converting it from a 32-bit float into JavaScript's native 64-bit double
    
    //var num2 = view2.getFloat32(0);
    //console.log(num2);
})