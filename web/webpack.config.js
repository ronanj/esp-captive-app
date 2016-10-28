var path = require('path');
var webpack = require('webpack');

module.exports = {
  entry: {
  	app: path.resolve(__dirname+"/src/")+"/app.ts",
  },
  output: {
    path: path.resolve(__dirname+"/../data/"),
    filename: 'bundle.js', 
    libraryTarget: "var",
    library: "app",
  }, 
  plugins: [
    new webpack.optimize.UglifyJsPlugin({ minimize: true }),
  ],
  resolve: {
      extensions: ['', '.webpack.js', '.web.js', '.ts', '.tsx', '.js']
  },
  module: {
      loaders: [
        { test: /\.tsx?$/, loader: 'ts-loader' },
        { test: /\.json$/, loader: 'json-loader' },
        { test: /\.css$/, loader: 'css-loader' },
        { test: /\.csv$/, loader: 'raw-loader' }
      ]
  },
};

