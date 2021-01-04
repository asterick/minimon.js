const fs = require('fs').promises;
const { Menu, dialog, BrowserWindow } = require('electron');
import isDev from 'electron-is-dev';

const isMac = process.platform === 'darwin'

const template = [
  // { role: 'appMenu' }
  ...(isMac ? [{
    label: app.name,
    submenu: [
      { role: 'about' },
      { type: 'separator' },
      { role: 'services' },
      { type: 'separator' },
      { role: 'hide' },
      { role: 'hideothers' },
      { role: 'unhide' },
      { type: 'separator' },
      { role: 'quit' }
    ]
  }] : []),
  // { role: 'fileMenu' }
  {
    label: 'File',
    submenu: [
        {
            label: "Open Image",
            accelerator: "CmdOrCtrl+O",
            click: async(e) => {
                const window = BrowserWindow.getFocusedWindow();
                const result = await dialog.showOpenDialog(window, {
                    title: "Open rom image",
                    filters: [
                        { name: "Pokemon mini rom", extensions: ["min"] },
                        { name: "All files", extensions: ["*"] }
                    ]
                });

                if (result.canceled) {
                    return ;
                }

                for (const fn of result.filePaths) {
                    window.webContents.send('load-file', await fs.readFile(fn));
                }
            }
        },
        {
            role: isMac ? 'close' : 'quit',
            accelerator: "CmdOrCtrl+Q"
        }
    ]
  },
  // { role: 'windowMenu' }
  {
    label: 'Window',
    submenu: [
      { role: 'minimize' },
      { role: 'zoom' },
      ...(isDev ? [{ type: 'separator' },{ role: 'toggleDevTools' }] : []),
      ...(isMac ? [
        { type: 'separator' },
        { role: 'front' },
        { type: 'separator' },
        { role: 'window' }
      ] : [
        { role: 'close' }
      ])
    ]
  },
  {
    role: 'help',
    submenu: [
      {
        label: 'Learn More',
        click: async () => {
          const { shell } = require('electron')
          await shell.openExternal('https://github.com/asterick/minimon.js')
        }
      }
    ]
  }
]

export const menu = Menu.buildFromTemplate(template);
