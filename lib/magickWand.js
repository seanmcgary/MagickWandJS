var magickWandNative	= require('../build/Release/magickWandJs.node'),
	path				= require('path'),
	_ 					= require('underscore');

var MagickWand = function(){
	
	return {
		resizeImage: function(options, callback){
			var defaults = {
				strip: true,
				width: 200,
				quality: 100,
				is_square: false
			};

			_.defaults(options, defaults);
		}
	}
}

module.exports = MagickWand;
