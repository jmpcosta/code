const {app,BrowserWindow} = require('electron');

var mainWindow= null;

app.on('ready', function () {
    mainWindow = new BrowserWindow( {width:500, height: 300} );
    mainWindow.loadURL( 'file://' + __dirname + '/index.html' );
});