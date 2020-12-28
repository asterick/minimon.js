import { app, BrowserWindow } from 'electron';
import isDev from 'electron-is-dev';
import updateApp from 'update-electron-app';

declare const EMULATOR_WEBPACK_ENTRY: string;

if (require('electron-squirrel-startup')) {
  app.quit();
}

updateApp();

const createWindow = (): void => {
  // Create the browser window.
  const mainWindow = new BrowserWindow({
    height: 600,
    width: 800,
    webPreferences: {
      devTools: isDev
    }
  });

  // and load the index.html of the app.
  mainWindow.loadURL(EMULATOR_WEBPACK_ENTRY);
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
