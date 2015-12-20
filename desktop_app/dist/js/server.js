var http = require('http');
var fs = require('fs');

const PORT = 3001;

function handleRequest(req, res){
    var data = "";

    console.log("[200] " + req.method + " to " + req.url);

    if (req.method == 'POST' && req.url == '/stream') {
        req.on('data', function(chunk) {
            data += chunk.toString();
            console.log("Received body data");
        });

        req.on('end', function() {
            fs.appendFile('./data/stream.in', data, 'ascii', function (err, done) {
                if(err) {
                    console.log(err);
                    return;
                }

                // empty 200 OK response for now
                console.log('The data is saved in the file system');
                console.log(data);
            });
        });
    }

    res.end('It Works!! Path Hit: ' + req.url);
}

//Create a server
var server = http.createServer(handleRequest);

//Lets start our server
server.listen(PORT, function(){
    //Callback triggered when server is successfully listening. Hurray!
    console.log("Server listening on: http://localhost:%s", PORT);
});