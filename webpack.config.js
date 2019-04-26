const path = require("path");
const webpack = require("webpack");

const mode = process.env.NODE_ENV || "development";

module.exports = {
    mode: mode,
    entry: './src/index.jsx',
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
                test: /\.glsl$/,
                use: [
                    {
                        loader: 'raw-loader',
                        options: {}
                    }
                ],
            },
            {
                test: /\.css$/,
                use: [
                    {
                        loader: 'style-loader'
                    },
                    {
                        loader: 'css-loader',
                        options: {
                            modules: true,
                            importLoaders: 1,
                            localIdentName: '[sha1:hash:hex:4]'
                        }
                    }
                ],
            },
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
