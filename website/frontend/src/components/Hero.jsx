import React from 'react';

function Hero({
  title,
  description,
  imageUrl,
  imageAlt = '',
  imageRight = true,
  showDescription = true,
  showButtons = true,
  primaryButton,
  secondaryButton,
  extraSections = [],
  mainImageHeight = "auto",
  withShadow = true,
  verticalPadding = "py-16 lg:py-24" // Nuova prop per gestire padding verticale
}) {
  return (
    <div className={`w-full flex items-center bg-cover bg-center bg-no-repeat ${verticalPadding}`}>
      <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-0 xl:px-0 w-full">
        <div className={`flex flex-col lg:flex-row items-center justify-between w-full ${imageRight ? '' : 'lg:flex-row-reverse'}`}>
          
          {/* Text section */}
          <div className="w-full lg:w-1/2 mb-8 lg:mb-0 lg:px-8">
            <h1 className="text-4xl lg:text-7xl font-extrabold text-gray-900 leading-tight pl-4 rounded-lg">
              {title}
            </h1>

            {showDescription && (
              <p className="text-xl mt-3 text-gray-800 p-4 rounded-lg">
                {description}
              </p>
            )}

            {showButtons && (
              <div className="mt-8 flex flex-wrap gap-4">
                {primaryButton && (
                  <a
                    href={primaryButton.href}
                    className="inline-flex items-center px-6 py-3 border border-transparent text-base font-medium rounded-md text-white bg-[#02d1ff] hover:bg-[#1ca2c0]"
                  >
                    {primaryButton.text}
                  </a>
                )}
                {secondaryButton && (
                  <a
                    href={secondaryButton.href}
                    className="inline-flex items-center px-6 py-3 border border-[#02d1ff] text-base font-medium rounded-md text-[#02d1ff] bg-white hover:bg-indigo-50"
                  >
                    {secondaryButton.text}
                  </a>
                )}
              </div>
            )}

            {extraSections.length > 0 && (
              <div className="space-y-4">
                {extraSections.map((section, index) => (
                  <div
                    key={index}
                    className="flex items-center space-x-2"
                    style={{
                      padding: section.padding || undefined,
                      margin: section.margin || undefined,
                    }}
                  >
                    <img
                      src={section.image}
                      alt={section.alt || ''}
                      style={{ height: section.imageHeight || '4rem' }}
                      className="object-cover rounded"
                    />
                    <p className="text-gray-700 text-base">{section.text}</p>
                  </div>
                ))}
              </div>
            )}
          </div>

          {/* Image section */}
          <div className="w-full lg:w-1/2 flex justify-center">
            <img
              src={imageUrl}
              alt={imageAlt}
              style={{ height: mainImageHeight || 'auto' }}
              className={`w-full object-contain rounded-xl ${withShadow ? 'drop-shadow-[0_10px_15px_rgba(0,0,0,0.2)]' : ''}`}
            />
          </div>

        </div>
      </div>
    </div>
  );
}

export default Hero;
