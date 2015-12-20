(function() {
  'use strict';

  angular
      .module('Home', ['ngModal'])
      .config(function(ngModalDefaultsProvider) {
        ngModalDefaultsProvider.set('option', 'value');
      })
      .controller('HomeController', ['$q', HomeController]);

  function HomeController($q) {
    var vm = this;

    /**
      * Gesture model:
      *
      * unique ID - {string},
      * name - {string},
      * data - [{x, y, z}] or [[{x, y, z}]],
      * command - {string}
      **/

    function checkConnection(){
      // TODO: Check if the server is collecting data from the device
    }

    vm.executeCommand = function(cmd){
      //var command = angular.element(ev.target).data('cmd');
      console.log(cmd);

      // execute the command via node.js
      var exec = require('child_process').exec;

      exec(cmd, function (error, stdout, stderr) {
        console.log('stdout: ' + stdout);
        console.log('stderr: ' + stderr);

        if (error !== null) {
          console.log('exec error: ' + error);
        }
      });
    };

    getGestures().then(function(data){
      console.log('gestures data');
      vm.gestures = data;
    });

    vm.editGesture = function(){
      //
    };

    vm.addNewGesture = function(){
      // open new window
      vm.dialogShown = true;
      vm.newGesture = {};

    };

    function getGestures() {
      var deffered = $q.defer();

      var filePath = './gestures.json';
      var fs = require('fs');
      var exec = require('child_process').exec;

      exec('python ./pydaemon/build_index.py', function (error, stdout, stderr) {
        console.log('stderr: ' + stderr);

        if (error !== null) {
          deffered.reject(error);
          console.log('exec error: ' + error);
        }

        fs.readFile(filePath, 'ascii', function (err, data) {
          console.log(data);
          vm.gestures = JSON.parse(data);
          console.log(vm.gestures);
          deffered.resolve(JSON.parse(data));
        });
      });

      return deffered.promise;
    }


      //return [{
      //  id: 1,
      //  name: 'elven bow',
      //  command: 'vlc --play-sound=applouse.wav'
      //}, {
      //  id: 2,
      //  name: "wave good-bye",
      //  command: "kill all webstorm"
      //}, {
      //  id: 3,
      //  name: "wave left-right to open favorite playlist in youtube with auto-play  ",
      //  command: "chromium 'https://www.youtube.com/watch?v=8Zx6RXGNISk&feature=youtu.be&t=8'"
      //}, {
      //  id: 4,
      //  name: "counter-clockwise circle",
      //  command: "amixer -q sset Master 10%-"
      //}, {
      //  id: 5,
      //  name: "clockwise circle",
      //  command: "amixer -q sset Master 10%+"
      //}];
  }
})();
