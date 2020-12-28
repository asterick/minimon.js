import { app, BrowserWindow } from 'electron';
import isDev from 'electron-is-dev';

declare const EMULATOR_WEBPACK_ENTRY: any;

if (require('electron-squirrel-startup')) {
  app.quit();
}

require('update-electron-app')();

const createWindow = (): void => {
  // Create the browser window.
  const mainWindow = new BrowserWindow({
    height: 600,
    width: 800,
  });

  // and load the index.html of the app.
  mainWindow.loadURL(EMULATOR_WEBPACK_ENTRY);
  if (isDev)
    mainWindow.webContents.openDevTools();
};

app.on('ready', createWindow);

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', () => {
  if (BrowserWindow.getAllWindows().length === 0) {
    createWindow();
  }
});
