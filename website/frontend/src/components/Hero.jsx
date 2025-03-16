import React from 'react';

const Hero = () => {
  return (
    <div 
      className="flex-grow flex items-center bg-cover bg-center bg-no-repeat" 
    >
      <div className="max-w-7xl mx-auto px-4 sm:px-6 lg:px-8 w-full pt-16">  
        <div className="flex flex-col lg:flex-row items-center justify-between h-full py-8 lg:py-0">
          
          {/* Left side with text */}
          <div className="w-full lg:w-1/2 mb-8 lg:mb-0 lg:pr-8">
            <h1 className="text-4xl lg:text-7xl font-extrabold text-gray-900 leading-tight p-4 rounded-lg font-grotesk">
              "I like to move it, move it"
            </h1>
            <p className="mt-4 text-xl text-gray-800 p-4 rounded-lg font-grotesk">
              Ciao, siamo dei ragazzi dell'istituto industriale informatico e abbiamo realizzato la scena iconica di Re Julien durante la sua festa
            </p>
            <div className="mt-8 font-grotesk">
              <a href="#" className="inline-flex items-center px-6 py-3 border border-transparent text-base font-medium rounded-md text-white bg-[#02d1ff] hover:bg-[#1ca2c0]">
                Scopri di più
              </a>
              <a href="#" className="ml-4 inline-flex items-center px-6 py-3 border border-[#02d1ff] text-base font-medium rounded-md text-[#02d1ff] bg-white hover:bg-indigo-50">
                Contattaci
              </a>
            </div>
          </div>
          
          {/* Right side with image */}
          <div className="w-full lg:w-1/2">
            <img 
              className="" 
              src="me_and_gloria.png" 
              alt="Team RoboCup" 
            />
          </div>

        </div>
      </div>
    </div>
  );
};

export default Hero;
