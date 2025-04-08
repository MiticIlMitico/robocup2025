import React from 'react';

function CircuitCodeSection({
  title,
  imageUrl,
  imageAlt = '',
  titlePadding = 'pt-8',
  imagePadding = 'pb-8',
  imageHeight = 'auto',
  codeTitle = 'Codice del Progetto',
  containerPaddingTop = 'pt-16',
  containerPaddingBottom = 'pb-16',
}) {
  return (
    <section className={`w-full bg-transparent h-auto ${containerPaddingTop} ${containerPaddingBottom}`}>
      <div className="max-w-7xl mx-auto w-full flex flex-col lg:flex-row items-center justify-center gap-16 px-4 sm:px-6 lg:px-8">

        {/* Sinistra: immagine del circuito */}
        <div className="w-full lg:w-1/2 flex flex-col items-center">
          <h2 className={`text-3xl lg:text-5xl font-extrabold text-gray-900 font-grotesk text-center ${titlePadding}`}>
            {title}
          </h2>
          <div className={`w-full flex justify-center ${imagePadding}`}>
            <img 
              src={imageUrl} 
              alt={imageAlt} 
              style={{ height: imageHeight }}
              className="w-full max-w-3xl object-contain rounded-xl"
            />
          </div>
        </div>

        {/* Destra: codice con effetto 3D, centrato perfettamente */}
        <div className="w-full lg:w-1/2 flex flex-col items-center">
          <h2 className={`text-3xl lg:text-5xl font-extrabold text-gray-900 font-grotesk text-center ${titlePadding}`}>
            {codeTitle}
          </h2>
          <div
            className="mt-6 w-full max-w-2xl flex justify-center"
            style={{
              perspective: '1000px',
              height: '450px',
            }}
          >
            <div
              className="h-full w-full rounded-[50px] transition-all duration-500 ease-in-out"
              style={{
                background: '#011522',
                transformStyle: 'preserve-3d',
                boxShadow:
                  'rgba(5, 71, 17, 0) 40px 50px 25px -40px, rgba(5, 71, 17, 0.2) 0px 25px 25px -5px',
              }}
              onMouseEnter={(e) => {
                e.currentTarget.style.transform = 'rotate3d(1, 1, 0, 30deg)';
                e.currentTarget.style.boxShadow =
                  'rgba(5, 71, 17, 0.3) 30px 50px 25px -40px, rgba(5, 71, 17, 0.1) 0px 25px 30px 0px';
              }}
              onMouseLeave={(e) => {
                e.currentTarget.style.transform = 'rotate3d(0, 0, 0, 0deg)';
                e.currentTarget.style.boxShadow =
                  'rgba(5, 71, 17, 0) 40px 50px 25px -40px, rgba(5, 71, 17, 0.2) 0px 25px 25px -5px';
              }}
            >
              {/* Finta barra header */}
              <div className="flex items-center space-x-2 px-6 py-6">
                <span className="w-3 h-3 rounded-full bg-[#ff605c]"></span>
                <span className="w-3 h-3 rounded-full bg-[#ffbd44]"></span>
                <span className="w-3 h-3 rounded-full bg-[#00ca4e]"></span>
              </div>
              {/* Codice visibile */}
              <div className="px-4 py-4 text-sm text-white font-mono whitespace-pre-wrap overflow-auto text-center">
                <code>{`function saluta() {
  console.log("Ciao dal bottone 3D!");
}

function balla() {
  console.log("Re Julien sta ballando!");
}`}</code>
              </div>
            </div>
          </div>
        </div>

      </div>
    </section>
  );
}

export default CircuitCodeSection;
