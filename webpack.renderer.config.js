const rules = require('./webpack.rules');
const plugins = require('./webpack.plugins');
const path = require('path');
const CopyWebpackPlugin = require('copy-webpack-plugin');
const assets = [ 'static' ];

rules.push({
  test: /\.css$/,
  use: [{ loader: 'style-loader' }, { loader: 'css-loader' }],
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
