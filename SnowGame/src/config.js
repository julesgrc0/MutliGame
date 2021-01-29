const fs = require("fs");
const path = require("path");
const log = require("./log");

module.exports = class Config {

    Settings = {
        PORT: 80,
        HOSTNAME: "default"
    };

    constructor() {
        this.VerfifyFiles();
    }

    Create(type, path, base, name) {
        if (type == "dir") {
            fs.mkdir(path, {}, (err) => {
                if (err) {
                    log.error("ERROR DIRECTORY", "Fail to create " + name + "  directory in " + base);
                } else {
                    log.warm("WARM DIRECTORY", "Has " + name + " been created in " + base);
                }
            });
        }
        else if (type == "file") {
            fs.writeFile(path, "", (err) => {
                if (err) {
                    log.error("ERROR FILE", "Fail to create " + name + " file in " + base);
                } else {
                    log.warm("WARM FILE", "Has " + name + " been created in " + base);
                }
            })
        }
    }

    VerfifyFiles() {
        let Content = path.resolve(__dirname, "content");
        let Log = path.resolve(__dirname, "log");
        let Game = path.resolve(__dirname,"game");
        
        let config = path.resolve(Content, "settings.conf");
        let info = path.resolve(Content,"README.md");
        let licence = path.resolve(Content,"LICENCE.md");


        if (!fs.existsSync(Content)) {
            this.Create("dir", Content, __dirname, "content");
        }
        if (!fs.existsSync(Log)) {
            this.Create("dir", Log, __dirname, "log");
        }
        if (!fs.existsSync(Game)) {
            this.Create("dir", Game, __dirname, "game");
        }

        if (!fs.existsSync(config)) {
            this.Create("file", config, __dirname, "settings");
        }
        if (!fs.existsSync(info)) {
            this.Create("file", info, __dirname, "info");
        }
        if (!fs.existsSync(licence)) {
            this.Create("file", licence, __dirname, "licence");
        }
    }



    ParseConfig(data) {
        let lines = data.split("\n");
        lines.map(line => {
            if (!line.startsWith("#")) {
                let element = line.split("=");
                if (element.length == 2) {
                    this.Settings[element[0]] = element[1];
                }
            }
        });
    }

    AddLog(value) {
        let date = new Date().toLocaleDateString();
        let logP = path.resolve(path.resolve(__dirname, "log"), date + ".log");
        fs.appendFile(logP, value + "\n", (err) => {
            if (err) {
                log.error("ERROR WRITE", "Fail to write in log file at " + path.resolve(__dirname, "log"));
            } else {
                log.win("READ", "Success to write log file");
            }
        });
    }

    ReadConfig() {
        return new Promise((res,rej) => {
            fs.readFile(path.resolve(path.resolve(__dirname, "content"), "settings.conf"), (err, data) => {
                if (err) {
                    log.error("ERROR READ", "Fail to read settings file in " + path.resolve(__dirname, "content"));
                } else {
                    log.win("READ", "Success to read settings file");
                    this.ParseConfig(data.toString());
                    res(this.Settings);
                }
            })
        })
    }
}