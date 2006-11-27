#ifndef PYRAMID_H
#define PYRAMID_H

#include <vector>
#include <string>

#include "Scaling.h"

namespace gil {
	template<typename I, typename T = double, typename S = TrivialScaler<I, T> >
	class Pyramid {
		public:
			typedef typename I::value_type value_type;

			Pyramid(const I &image)
			{
				size_t width = image.width();
				size_t height = image.height();

				my_pyramids.push_back(image);

				while (width > 1 && height > 1) {
					width = (width+1) / 2;
					height = (height+1) / 2;
					const I &img = my_pyramids[ my_pyramids.size()-1 ];
					my_pyramids.push_back(
						scale(
							img, 
							S(
								static_cast<T>(width)/img.width(), 
								static_cast<T>(height)/img.height()
							),
							width,
							height
						)
					);
				}

				/*
				string name = "p";
				for (size_t i = 0; i < my_pyramids.size(); ++i) {
					write(my_pyramids[i], name + ".png");					
					name += "p";
				}
				*/
			}

			value_type pixel(T layer, T x, T y) const
			{

			}

			value_type texel(T layer, T x, T y) const
			{

			}

		protected:

		private:
			std::vector<I> my_pyramids;
	};

}

#endif
