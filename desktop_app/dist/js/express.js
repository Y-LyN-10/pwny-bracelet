var fs = require('fs');
var express = require('express');
var bodyParser = require('body-parser');
var app = express();
var inputPath = './stream.in';
var stream = [];
var maxStreamLength = 250;

app.use(bodyParser.json()); // for parsing application/json
app.use(bodyParser.urlencoded({ extended: true })); // for parsing application/x-www-form-urlencoded

// execute the command via node.js
var exec = require('child_process').exec,
    child;

function runDaemon(){
    exec('python ./pydaemon/main.py', function (error, stdout, stderr) {
        console.log('stdout: ' + stdout);
        console.log('stderr: ' + stderr);

        if (error !== null) {
            console.log('exec error: ' + error);
        }
    });
}

runDaemon();

app.post('/stream', function (req, res) {
    console.log("[200] " + req.method + " to " + req.url);
    var data = req.body;

    for (var k in data){
        stream.push(k);
    }

    if(stream.length > maxStreamLength){
        stream.slice(stream.length  - maxStreamLength, stream.length);
    }

    fs.writeFile(inputPath, stream.join('\r\n'), 'ascii', function (err, done) {
        if(err){
            res.status(400);
            res.send('Bad Request');
        } else {
            console.log(stream);
            res.status(200);
            res.send('Saved data');
        }
    });
});

app.get('/cp', function (req, res) {
    console.log("[200] " + req.method + " to " + req.url);

    exec('cp ./stream.in ./data/tmp/' + Date.now(), function (error, stdout, stderr) {
        console.log('stdout: ' + stdout);
        console.log('stderr: ' + stderr);

        if (error !== null) {
            console.log('exec error: ' + error);
        }

        res.send('OK');
    });
});

var server = app.listen(3001, function () {
    var host = server.address().address;
    var port = server.address().port;

    console.log('Example app listening at http://%s:%s', host, port);
});
