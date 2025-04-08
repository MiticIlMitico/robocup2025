import React from 'react';

function DoubleFeature({
  items = [],
  verticalPadding = "py-20"
}) {
  return (
    <div className={`w-full h-auto ${verticalPadding} bg-transparent`}>
      <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8">
        <div className="grid grid-cols-1 lg:grid-cols-2 gap-12 relative">
          {/* Progetto 1 */}
          <div className="flex flex-col items-center text-center px-4 lg:px-8">
            <h2 className="text-4xl lg:text-6xl font-extrabold text-gray-900 leading-tight mb-4">
              {items[0]?.title}
            </h2>
            <p className="text-xl text-gray-700 mb-6 max-w-xl">
              {items[0]?.description}
            </p>
            <img
              src={items[0]?.imageUrl}
              alt={items[0]?.imageAlt || ''}
              className="w-full max-h-[400px] object-contain rounded-xl drop-shadow-md"
            />
          </div>

          {/* Separatore */}
          <div className="absolute left-1/2 top-0 transform -translate-x-1/2 h-full w-px bg-gray-300 hidden lg:block"></div>
          <div className="block lg:hidden border-t border-gray-300 my-8 col-span-2"></div>

          {/* Progetto 2 */}
          <div className="flex flex-col items-center text-center px-4 lg:px-8">
            <h2 className="text-4xl lg:text-6xl font-extrabold text-gray-900 leading-tight mb-4">
              {items[1]?.title}
            </h2>
            <p className="text-xl text-gray-700 mb-6 max-w-xl">
              {items[1]?.description}
            </p>
            <img
              src={items[1]?.imageUrl}
              alt={items[1]?.imageAlt || ''}
              className="w-full max-h-[400px] object-contain rounded-xl drop-shadow-md"
            />
          </div>
        </div>
      </div>
    </div>
  );
}

export default DoubleFeature;
