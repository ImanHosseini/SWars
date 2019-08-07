const { app, BrowserWindow, ipcMain } = require('electron')
const net = require('net');
var client = new net.Socket();

function createWindow() {
    // Create the browser window.
    let win = new BrowserWindow({
        width: 1200,
        height: 800,
        webPreferences: {
            nodeIntegration: true
        }
    })

    win.removeMenu()
    win.setResizable(false)
    win.webContents.openDevTools()
    // and load the index.html of the app.
    win.loadFile('index.html')

    const SERVER_IP = '127.0.0.1';
    client.connect(27015, SERVER_IP, function () {
        console.log('Connected');
        client.write('Hello, server! Love, Client.');
    });

    var hexChar = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"];

    function byteToHex(b) {
        return hexChar[(b >> 4) & 0x0f] + hexChar[b & 0x0f];
    }

    var tmpBuf = Buffer.alloc(0);
    var expecting = 0;
    var got = 0;
    
    client.on('data', function (data) {
        dd(data);
        var n = data.length;
        var cur = 0;
        var left = n;
        console.log('rcvd:' + data.length);
        var data_arr = [];
        for (i = 0; i < n; i++) {
            data_arr[i] = data[i];
        }
        if (expecting != 0) {
            if (expecting <= n) {
                console.log('exp IS: ' + expecting);
                var buf2 = data.slice(cur, cur + expecting);
                buf2.copy(tmpBuf, got);
                console.log("tbuf:" + tmpBuf.toString());
                win.webContents.send('message', tmpBuf);
                tmpBuf = Buffer.alloc(0);
                cur = cur + expecting;
                left = n - expecting;
                expecting = 0;
                got = 0;
            } else {
                console.log('exp IS non zero');
                var buf2 = data.slice(cur, cur + n);
                buf2.copy(tmpBuf, got);
                cur = cur + n;
                left = 0;
                expecting = expecting - n;
                got = got + n;
            }
        
        }
        
        while (left != 0) {
            console.log('cur at: '+cur);
                var size_header = new ArrayBuffer(1);
                var v_size_header = new DataView(size_header);
                v_size_header.setInt8(0, data[cur]);
                var size = v_size_header.getInt8(0);
                cur = cur + 1;
                left = n - cur;
                if (size <= left) {
                    console.log(cur + "->" + (cur + size));
                    console.log(data_arr.slice(cur, cur + size).toString());
                    win.webContents.send('message',data.slice(cur, cur + size));
                   // win.webContents.send('message', 'hi');
                    cur = cur + size;
                    left = n - cur;
                } else if(left>0){
                    tmpBuf = Buffer.alloc(size);
                    console.log("size is: " + size);
                    var buf2 = data.slice(cur, cur + left);
                    buf2.copy(tmpBuf, 0);
                    expecting = size - left;
                    got = left;
                    left = 0;
                }
               
            }
           
    

        //console.log('Received: ' + data.length);
        //console.log("bytes: " + byteToHex(data[1]) + " " + byteToHex(data[2]) + " " + byteToHex(data[3]) + " " + byteToHex(data[4]));
        //// Create a buffer
        //var buf = new ArrayBuffer(4);
        //var buf2 = new ArrayBuffer(4);
        //// Create a data view of it
        //var view = new DataView(buf);
        //var view2 = new DataView(buf2);
        //// set bytes
        //view.setInt8(0, data[0]);
        //view2.setUint8(3, data[1]);
        //view2.setUint8(2, data[2]);
        //view2.setUint8(1, data[3]);
        //view2.setUint8(0, data[4]);
        //// Read the bits as a float; note that by doing this, we're implicitly
        //// converting it from a 32-bit float into JavaScript's native 64-bit double
        //var num = view.getInt8(0);
        //var num2 = view2.getFloat32(0);
        //// Done
        //console.log("num is:"+num+"float: "+num2);
     
    });

    function dd(data) {
        str = "";
        for (i = 0; i < data.length; i++) {
            
            str += i + ":" + byteToHex(data[i])+" ";
        }
        console.log(str);
    }

    client.on('close', function () {
        console.log('Connection closed');
    });
}



ipcMain.on('asynchronous-message', (_, message) => {
    const buf = new Uint8Array(1);
    buf[0] = message;
    console.log("BUF IS:"+buf.toString());
    client.write(buf);
});

app.on('ready', createWindow)