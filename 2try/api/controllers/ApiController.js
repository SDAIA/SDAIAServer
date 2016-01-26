/*jshint enforceall: true, esnext: true, node: true*/

/**
 * ApiController
 *
 * @description :: Server-side logic for managing apis
 * @help        :: See http://sailsjs.org/#!/documentation/concepts/Controllers
 */

module.exports = {

  'getApiInfo': function (req, res){
    return res.json({
      name: "SDAIA Api",
      version: "0.0.0",
      dateOfRelease: "2016/01/26"
    });
  }
};

