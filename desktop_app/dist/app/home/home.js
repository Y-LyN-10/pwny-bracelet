(function() {
  'use strict';

  angular
      .module('Home', [])
      .controller('HomeController', ['$scope', HomeController]);

  function HomeController() {
    var vm = this;
    vm.gestures = getGestures();

    function getGestures(){
      return [{
        name: 'hand to left',
        command: 'vlc --next-song'
      }, {
        name: "hand up - down - up",
        command: "gulp serve presentation"
      }, {
        name: "wave left-right",
        command: "turn off the computer"
      }];
    }
  }
})();
