var exec = require('child_process').exec,
    child;

child = exec('netcat -l 3001 > recv.txt',
    function (error, stdout, stderr) {
        console.log('stdout: ' + stdout);
        console.log('stderr: ' + stderr);
        if (error !== null) {
            console.log('exec error: ' + error);
        }
    });