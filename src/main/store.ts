
const { app, ipcMain } = require('electron');

const path = require('path');
const fs = require('fs');

const userDataPath = app.getPath('userData');

ipcMain.on('load-data', (event, name:string, encoding:string) => {
    try {
        event.returnValue = { data: fs.readFileSync(path.join(userDataPath, name), encoding) };
    } catch(e) {
        event.returnValue = { err: e.message };
    }
});

ipcMain.on('save-data', (event, name:string, data:any) => {
    try {
        fs.writeFileSync(path.join(userDataPath, name), data);
        event.returnValue = "okay";
    } catch(e) {
        event.returnValue = { err: e.message };
    }
});
