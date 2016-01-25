/*jshint enforceall: true, esnext: true, node: true*/

/**
 * ApiController
 *
 * @description :: Server-side logic for managing apis
 * @help        :: See http://sailsjs.org/#!/documentation/concepts/Controllers
 */

module.exports = {

  'getWelcomeMessage': function (req, res){
    return res.json({
      message: "Welcome to SDAIA api.",
      version: "0.0.0 - 20160125"
    });
  }
};

