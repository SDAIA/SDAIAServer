/*jshint enforceall: true, node: true*/
// server.js

// BASE SETUP
// =============================================================================

// call the packages we need
var express    = require('express'),       // call express
    app        = express(),                 // define our app using express
    bodyParser = require('body-parser'),
    mongoose   = require('mongoose'),
    swaggerize = require('swaggerize-express');

// configure app to use bodyParser()
// this will let us get the data from a POST
app.use(bodyParser.urlencoded({ extended: true }));
app.use(bodyParser.json());

var port = process.env.PORT || 8080;        // set our port

// configure mongoose
mongoose.connect('mongodb://localhost:27017/sdaia');

// Swagger API
app.use(swaggerize({
    api: require('./sdaia-api.json'),
    docspath: '/api-docs',
    handlers: './handlers'
}));

// ROUTES FOR OUR API
// =============================================================================
//var router = express.Router();              // get an instance of the express Router

// test route to make sure everything is working (accessed at GET http://localhost:8080/api)
//router.get('/', function(req, res) {
//    res.json({ message: 'Welcome to SDAIA.' });
//});

// more routes for our API will happen here

// REGISTER OUR ROUTES -------------------------------
// all of our routes will be prefixed with /api
//app.use('/sdaia-api', router);

// START THE SERVER
// =============================================================================
app.listen(port);
console.log('Listening on port ' + port);
