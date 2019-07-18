const path = require("path");
const webpack = require("webpack");

module.exports = {
    target: 'node',
    mode: "development",
    entry: './src/system/index.js',
    output: {
        libraryTarget: 'umd',
        path: path.join(__dirname, 'public'),
        filename: 'system.compiled.js',
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
