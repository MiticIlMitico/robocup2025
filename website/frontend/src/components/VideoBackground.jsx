import React, { useState, useEffect } from 'react';

export default function VideoBackground() {
  const [isVisible, setIsVisible] = useState(false);
  const [isHovering, setIsHovering] = useState(false);

  useEffect(() => {
    // Aggiungi un piccolo ritardo per l'animazione di entrata
    const timer = setTimeout(() => {
      setIsVisible(true);
    }, 300);

    return () => clearTimeout(timer);
  }, []);

  return (
    <div className="relative w-full h-screen overflow-hidden">
      {/* Video di sfondo */}
      <video
        className="absolute top-0 left-0 w-full h-full object-cover"
        autoPlay
        loop
        muted
        playsInline
      >
        <source src="/0316_encoder.mp4" type="video/mp4" />
        Il tuo browser non supporta il tag video.
      </video>

      {/* Overlay con gradiente avanzato */}
      <div className="absolute inset-0 bg-gradient-to-b from-[#2e3a3d]/80 via-[#2e3a3d]/70 to-[#2e3a3d]/90"></div>

      {/* Contenuto sopra il video */}
      <div className="absolute inset-0 flex flex-col justify-center items-center px-4">
        <div className={`transform transition-all duration-1000 ${isVisible ? 'translate-y-0 opacity-100' : 'translate-y-10 opacity-0'} text-center`}>
          {/* Logo con effetto glow */}
          <div className="flex-shrink-0 flex items-center justify-center mb-6">
            <img 
              className="h-48 w-auto drop-shadow-[0_0_15px_rgba(0,0,0,0.5)]" 
              src="madagabot.png" 
              alt="Madagabot Logo" 
            />
          </div>
          
          {/* Sottotitolo con stile migliorato e ombra per leggibilità */}
          <p className="text-lg md:text-xl text-white max-w-2xl mx-auto mb-10 leading-relaxed text-center font-medium drop-shadow-[0_2px_4px_rgba(0,0,0,0.8)] text-shadow">
            Ciao, siamo i Madagabot e abbiamo replicato la scena iconica di Re Julien,
            ma in versione <span className="text-[#02d1ff] font-bold">dark techno</span>
          </p>
          
          {/* Bottone con effetti hover migliorati e centrato */}
          <div className="flex justify-center">
            <button 
              className="relative overflow-hidden px-8 py-4 rounded-lg bg-[#02d1ff] text-white font-medium text-lg transition-all duration-500 group mx-auto"
              onMouseEnter={() => setIsHovering(true)}
              onMouseLeave={() => setIsHovering(false)}
            >
              {/* Effetto onda al hover */}
              <span className={`absolute w-full h-full top-0 left-0 bg-gradient-to-r from-[#02d1ff] via-[#1ca2c0] to-[#02d1ff] transform ${isHovering ? 'translate-x-0' : '-translate-x-full'} transition-transform duration-700 opacity-60`}></span>
              
              {/* Effetto glow */}
              <span className="absolute inset-0 rounded-lg shadow-[0_0_15px_rgba(2,209,255,0.7)] opacity-0 group-hover:opacity-100 transition-opacity duration-500"></span>
              
              {/* Testo e bordo scintillante */}
              <span className="relative z-10 flex items-center justify-center">
                Scopri di più
                <svg className="w-5 h-5 ml-2 transform group-hover:translate-x-1 transition-transform duration-300" fill="none" stroke="currentColor" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
                  <path strokeLinecap="round" strokeLinejoin="round" strokeWidth="2" d="M14 5l7 7m0 0l-7 7m7-7H3"></path>
                </svg>
              </span>
            </button>
          </div>
        </div>
      </div>
      
      {/* Decorazioni geometriche */}
      <div className="absolute bottom-10 left-10 w-32 h-32 border border-[#02d1ff]/20 rounded-full opacity-20 animate-pulse"></div>
      <div className="absolute top-20 right-20 w-16 h-16 border border-[#02d1ff]/20 rounded-full opacity-20 animate-pulse" style={{animationDelay: '1s'}}></div>
    </div>
  );
}