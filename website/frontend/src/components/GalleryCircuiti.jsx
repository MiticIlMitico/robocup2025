import React from 'react';

function GalleryCircuiti({ images = [], height = "h-64", title = "Circuiti del progetto" }) {
  return (
    <div className={`w-full bg-transparent py-8 px-4`}>
      <h2 className="text-3xl font-bold text-gray-800 mb-6 text-center">{title}</h2>

      <div className={`w-full overflow-x-auto ${height} scroll-smooth`}>
        <div className="flex gap-6 items-center">
          {images.map((src, index) => (
            <img
              key={index}
              src={src}
              alt={`Circuito ${index + 1}`}
              className="h-full max-w-none rounded-xl shadow-md object-contain"
            />
          ))}
        </div>
      </div>
    </div>
  );
}

export default GalleryCircuiti;
