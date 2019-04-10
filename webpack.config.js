const path = require("path");
const webpack = require("webpack");

const mode = process.env.NODE_ENV || "development";

module.exports = {
    mode: mode,
    entry: './client/index.jsx',
    output: {
        path: path.join(__dirname, 'public'),
        filename: 'app.js',
    },
    devServer: {
        contentBase: path.join(__dirname, 'public'),
        compress: true,
        port: 9000
    },
    plugins: [
        new webpack.IgnorePlugin(/^text-encoding$/),
        new webpack.IgnorePlugin(/^fs$/)
    ],
    module: {
        rules: [
            {
                test: /\.jsx?$/,
                exclude: /node_modules/,
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
