const rules = require('./webpack.rules');
const plugins = require('./webpack.plugins');
const path = require('path');
const CopyWebpackPlugin = require('copy-webpack-plugin');
const assets = [ 'static' ];

rules.push({
  test: /\.less$/,
  use: [
      'style-loader',
      {
          loader: 'css-loader',
          options: {
              modules: true
          }
      },
      {
          loader: 'less-loader',
          options: {
              lessOptions: {
                  paths: [path.resolve(__dirname, 'node_modules')]
              }
          }
      }
  ],
}, {
  test: /\.csv$/i,
  use: './loaders/table-loader'
});

// Add asset mapping
plugins.push(
  new CopyWebpackPlugin({
    patterns: assets.map((asset) => {
      return {
        'from': path.resolve(__dirname, 'src', asset),
        'to': asset
      }
    }),
  })
);

module.exports = {
  module: {
    rules,
  },
  plugins: plugins,
  resolve: {
    extensions: ['.js', '.ts', '.jsx', '.tsx', '.css']
  },
};
