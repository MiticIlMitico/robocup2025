import React from 'react';

function Hero({
  title,
  description,
  imageUrl,
  imageAlt = '',
  imageRight = true,
  primaryButton,
  secondaryButton,
}) {
  return (
    <div className="flex-grow flex items-center bg-cover bg-center bg-no-repeat">
      <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 w-full pt-16">  
        <div className={`flex flex-col lg:flex-row items-center justify-between h-full py-8 lg:py-0 ${imageRight ? '' : 'lg:flex-row-reverse'}`}>
          
          {/* Text */}
          <div className="w-full lg:w-1/2 mb-8 lg:mb-0 lg:px-8">
            <h1 className="text-4xl lg:text-7xl font-extrabold text-gray-900 leading-tight p-4 rounded-lg font-grotesk">
              {title}
            </h1>
            <p className="mt-4 text-xl text-gray-800 p-4 rounded-lg font-grotesk">
              {description}
            </p>
            <div className="mt-8 font-grotesk flex flex-wrap gap-4">
              {primaryButton && (
                <a href={primaryButton.href} className="inline-flex items-center px-6 py-3 border border-transparent text-base font-medium rounded-md text-white bg-[#02d1ff] hover:bg-[#1ca2c0]">
                  {primaryButton.text}
                </a>
              )}
              {secondaryButton && (
                <a href={secondaryButton.href} className="inline-flex items-center px-6 py-3 border border-[#02d1ff] text-base font-medium rounded-md text-[#02d1ff] bg-white hover:bg-indigo-50">
                  {secondaryButton.text}
                </a>
              )}
            </div>
          </div>
          
          {/* Image */}
          <div className="w-full lg:w-1/2">
            <img 
              src={imageUrl} 
              alt={imageAlt} 
              className="w-full h-auto object-contain"
            />
          </div>

        </div>
      </div>
    </div>
  );
}

export default Hero;
