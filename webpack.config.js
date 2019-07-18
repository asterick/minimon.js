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
                test: /\.s?[ac]ss$/,
                use: [
                    'style-loader',
                    { 
                        loader: 'css-loader',
                        options: {
                            modules: true
                        }
                    },
                    {
                        loader: 'sass-loader',
                        options: {
                            includePaths: [
                                path.join(__dirname, 'node_modules')
                            ]
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
