const path = require("path");
const webpack = require("webpack");

const mode = process.env.NODE_ENV || "development";

module.exports = {
    mode: mode,
    entry: './client/index.jsx',
    output: {
        path: path.join(__dirname, './assets'),
        filename: 'app.js',
    },
    plugins: [
        new webpack.DefinePlugin({
            'process.env': {
                'NODE_ENV': JSON.stringify(mode)
            }
        }),
        new webpack.IgnorePlugin(/^text-encoding$/),
        new webpack.IgnorePlugin(/^fs$/)
    ],
    module: {
        rules: [
            {
                test: /\.jsx?$/,
                use: [ { loader: 'babel-loader' } ],
            }
        ]
    },
    resolve: {
        extensions: ['.js', '.jsx']
    }
};

if (mode !== 'production') {
    module.exports.resolve.alias = {
        inferno: __dirname + "/node_modules/inferno/dist/index.dev.esm.js"
    }
}
