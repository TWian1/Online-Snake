const express = require('express');
const app = express();
const port = 6873;
const app2 = express();
const port2 = 80;
var curip, parameters, curindex, parameterssplit, out, lout, leaderboard, oldlout;
var clients = [];
var data = [];
app.get('/:id', function(req, res){
    try{
    if (req.params.id == 'favicon.ico'){return;}
    try{
    lout = "Players: " + data.length + " --- ";
    leaderboard = [];
    for (let user of data){leaderboard.push([user.max, user.ip]);}
    for (let user of data){lout += user.ip + " ( " + user.username + " ) >>> " + user.max.toString() + "  |  ";}
    lout = lout.substring(0, lout.length-5);}
    catch{};
    if (req.params.id == 'test'){
        out = "";
        for (let user of data){
            for (let position of user.pos){
                if (isNaN(position[0]) || isNaN(position[1]) || isNaN(parseFloat(position[0])) || isNaN(parseFloat(position[1]))){throw new TypeError('Coord is not a number')}
                out += position[0].toString() + "+" + position[1].toString();
                out += "_";
            }
        }
        out = out.substring(0, out.length-1);
        res.send(out);
        return;
    }
    if (req.params.id == 'users'){
        out = "Players: " + data.length + " --- ";
        for (let user of data){out += user.ip + " ( " + user.username + " )" + "  |  "}
        out = out.substring(0, out.length-5);
        console.log(out)
        res.send(out);
        return;
    }
    if (req.params.id == 'blank'){
        curip = req.ip.replaceAll(':', '').replaceAll('f', '');
        curindex = data.findIndex(obj => obj.ip === curip);
        data[curindex].pos = [];
        out = "";
        for (let user of data){
            for (let position of user.pos){
                if (isNaN(position[0]) || isNaN(position[1]) || isNaN(parseFloat(position[0])) || isNaN(parseFloat(position[1]))){throw new TypeError('Coord is not a number')}
                out += position[0].toString() + "+" + position[1].toString();
                out += "_";
            }
        }
        out = out.substring(0, out.length-1);
        res.send(out);
        return;
    }
    if (req.params.id == 'leaderboard'){res.send(lout);return;}
    if (req.params.id == 'quit'){
        if (data.some(obj => obj.ip === curip)){
            curip = req.ip.replaceAll(':', '').replaceAll('f', '');
            curindex = data.findIndex(obj => obj.ip === curip);
            console.log("IP: " + curip + " ( " + data[curindex].username +  " ) has quit.  Players: " + (data.length - 1).toString());
            data.splice(curindex, 1)
            res.send("done");
            return;
        }
        res.send('err Not Singed in')
        return;
    }
    curip = req.ip.replaceAll(':', '').replaceAll('f', '');
    if (!(data.some(obj => obj.ip === curip))){
        data.push({ip: curip, username: "No Username", pos: [], max: 0});
        curindex = data.findIndex(obj => obj.ip === curip);
        if (req.params.id[0] == 'u'){
            data[curindex].username = req.params.id.split(">")[1].replaceAll('_', ' ')
            console.log("IP: " + curip + " ( " + data[curindex].username +  " ) has joined.  Players: " + data.length);
            out = "";
            for (let user of data){
                for (let position of user.pos){
                    if (isNaN(position[0]) || isNaN(position[1]) || isNaN(parseFloat(position[0])) || isNaN(parseFloat(position[1]))){throw new TypeError('Coord is not a number')}
                    out += position[0].toString() + "+" + position[1].toString();
                    out += "_";
                }
            }
            out = out.substring(0, out.length-1);
            res.send(out);
            return;
        }
        console.log("IP: " + curip + " (" + data[curindex].username +  ") has joined Players: " + data.length);
    }
    curindex = data.findIndex(obj => obj.ip === curip);
    if (req.params.id[0] == 'u'){
        data[curindex].username = req.params.id.split(">")[1].replaceAll('_', ' ')
        out = "";
        for (let user of data){
            for (let position of user.pos){
                if (isNaN(position[0]) || isNaN(position[1]) || isNaN(parseFloat(position[0])) || isNaN(parseFloat(position[1]))){throw new TypeError('Coord is not a number')}
                out += position[0].toString() + "+" + position[1].toString();
                out += "_";
            }
        }
        res.send(out.substring(0, out.length-1));
        return;
    }
    if (curindex == -1){throw new TypeError('Add IP failed')}
    parameters = req.params.id;
    parameterssplit = parameters.split("_");
    data[curindex].pos = [];
    for (let xy of parameterssplit){data[curindex].pos.push([xy.split("+")[0], xy.split("+")[1]]);}
    if(data[curindex].pos.length > data[curindex].max){data[curindex].max = data[curindex].pos.length;}
    out = "";
    for (let user of data){
        for (let position of user.pos){
            if (isNaN(position[0]) || isNaN(position[1]) || isNaN(parseFloat(position[0])) || isNaN(parseFloat(position[1]))){throw new TypeError('Coord is not a number')}
            out += position[0].toString() + "+" + position[1].toString();
            out += "_";
        }
    }
    res.send(out.substring(0, out.length-1));
    }
    catch(e){
        console.log('\x1b[4m\x1b[31m', 'Error: ' + e, '\x1b[0m', " IP: " + curip + " MESSAGE: " + req.params.id)
        res.send('err ' + e);
        return;
    }
});
app2.get('/', (req, res) => {
    res.setHeader('Content-Type', 'text/event-stream');
    res.setHeader('Cache-Control', 'no-cache');
    res.setHeader('Connection', 'keep-alive');
    if (lout != undefined){res.write(`LEADERBOARD SNAKE GAME:\n\n${lout}\n\n`);}
    else{res.write("LEADERBOARD SNAKE GAME:\n\n");}
    const clientId = Date.now();
    const newClient = { id: clientId, res };
    clients.push(newClient);
    req.on('close', () => {
        clients = clients.filter(client => client.id !== clientId);
    });
});
setInterval(() => {
    if (lout != undefined && lout != oldlout){
    oldlout = lout;
    clients.forEach(client => {client.res.write(`${lout}\n\n`);});}}, 1000);
app.listen(port, function (err){
    if (err) console.log(err);
    console.log('\x1b[32m', "server listening on port", '\x1b[0m', port);
})
app2.listen(port2, function (err){
    if (err) console.log(err);
    console.log('\x1b[32m', "server2 listening on port", '\x1b[0m', port2);
})