const express = require("express");
const http = require("http");
const ip = require("./src/ip")();
const cmd = require("./src/log");
const readline = require("readline");
const Config = require("./src/config");

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

if (ip && ip['Wi-Fi']) {
    const config = new Config();
    config.ReadConfig().then(settings => {
        const app = express();
        const server = http.createServer(app);
        const io = require('socket.io')(server);
        
        let url = "http://" + ip['Wi-Fi'] + "/";
        let port = 80;

        if(settings['HOSTNAME'] && settings['HOSTNAME'] != "default"){
            url = settings['HOSTNAME'];
            app.set("hostname", settings['HOSTNAME']);
        }else{
            app.set("hostname",ip['Wi-Fi']);
        }

        if(settings['PORT'] && parseInt(settings['PORT']) != 0){
            port = parseInt(settings['PORT']);
        }

        app.use(express.static('view'));

        let PlayerList = [];

        function SendPlayerInfo(expect,name,value){
            PlayerList.map(p=>{
                if(p!=expect){
                    p.emit(name,value);
                }
            })
        }

        io.on('connection', (socket) => {
            PlayerList.push(socket);
            console.log("[PLAYER] "+PlayerList.length);

            socket.on("[move:before]",(value)=>{
                SendPlayerInfo(socket,"[move:before]",value)
            })

            socket.on("[move:after]",(value)=>{
                SendPlayerInfo(socket,"[move:after]",value)
            })

            socket.on("[element:create]",(value)=>{
                SendPlayerInfo(socket,"[element:create]",value)
            })

            socket.on("[element:remove]",(value)=>{
                SendPlayerInfo(socket,"[element:remove]",value)
            })

            socket.on("disconnect",()=>{
                PlayerList = PlayerList.filter(it => it != socket);
                console.log("[PLAYER] "+PlayerList.length);
            })

        });

        app.get("/*", (req, res) => {
            config.AddLog("[GET] at " + new Date().toLocaleTimeString() + " on " + req.url );
        });
        server.listen(port, () => {
            config.AddLog("[SERVER] Start at " + new Date().toLocaleDateString() + " on " + url)
            cmd.log("SERVER", "Success to start on " + url)
        })
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