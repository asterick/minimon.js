const express = require("express");

const logging = require("./logging");
const Config = require("../config.json");

// === Main ===
const app = express();

app.engine('html', require('ejs').renderFile);

app.get("/", (req, res) => {
	res.render('index.html', {
	});
});

if (Config.environment === 'development') {
    const webpack = require("webpack");
    const compiler = webpack(require("../webpack.config.js"));

    compiler.watch({}, (err, stats) => {
        if (err) {
            logging("error", err);
        }
    });

	app.use(express.static('assets'));
}

app.listen(Config.server.port, () => {
    logging("debug", `Server started on port ${Config.server.port}`);
});
