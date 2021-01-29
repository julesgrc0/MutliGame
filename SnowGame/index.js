const express = require("express");
const http = require("http");
const { networkInterfaces } = require('os');
const colors = require("colors");
const readline = require("readline");
const fs = require("fs");
const ip = (function getIP(){
    const nets = networkInterfaces();
    let results = {};
    for (const name of Object.keys(nets)) {
        for (const net of nets[name]) {
            if (net.family === 'IPv4' && !net.internal) {
                if (!results[name]) {
                    results[name] = [];
                }
                results[name].push(net.address);
            }
        }
    }
    return results;
})();
const cmd = {
    log:(title, msg)=> 
    {
        console.log(colors.bgBlue(colors.black(title)) + " " + msg);
    },
    error:(title, msg) =>
    {
        console.log(colors.bgRed(colors.black(title)) + " " + msg);
    },
    warm:(title, msg) =>
    {
        console.log(colors.bgYellow(colors.black(title)) + " " + msg);
    },
    win:(title,msg)=>{
        console.log(colors.bgGreen(colors.black(title)) + " " + msg);
    },
}

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

if (ip && ip['Wi-Fi']) {
        const app = express();
        const server = http.createServer(app);
        const io = require('socket.io')(server);
        
        let url = "http://" + ip['Wi-Fi'] + "/";
        let port = 80;
        app.set("hostname",ip['Wi-Fi']);
        app.use(express.static('view'));

        let PlayerList = [];

        function SendPlayerInfo(expect,name,value){
            PlayerList.map(p=>{
                if(p!=expect){
                    p.emit(name,value);
                }
            })
        }
        let ballCount = 0;
        io.on('connection', (socket) => {
            PlayerList.push(socket);
            console.log("[GAME]","User connexion, count: "+PlayerList.length+"")

            socket.on("[move:before]",(value)=>{
                SendPlayerInfo(socket,"[move:before]",value)
            })

            socket.on("[move:after]",(value)=>{
                SendPlayerInfo(socket,"[move:after]",value)
            })

            socket.on("[element:create]",(value)=>{
                ballCount++;
                console.time(value.id);
                console.log("[GAME]","Create new SnowBall ("+value.x+";"+value.y+"), count: "+ballCount);
                SendPlayerInfo(socket,"[element:create]",value)
            })

            socket.on("[element:remove]",(value)=>{
                console.timeEnd(value.id);
                ballCount--;
                console.log("[GAME]","Remove SnowBall, count: "+ballCount);
                SendPlayerInfo(socket,"[element:remove]",value)
            })

            socket.on("disconnect",()=>{
                PlayerList = PlayerList.filter(it => it != socket);
                console.log("[GAME]","User disconnexion, count: "+PlayerList.length+"")
            })

        });

        app.get("/*", (req, res) => {
        });
        server.listen(port, () => {
            cmd.log("SERVER", "Success to start on " + url)
        })
} else {
    cmd.error("ERROR WIFI", "No Wifi connexion found.\n");
    Object.keys(ip).map(item => {
        console.log("[" + item + "] = " + ip[item]);
    })
    rl.question('', () => {
        rl.close();
    });
}