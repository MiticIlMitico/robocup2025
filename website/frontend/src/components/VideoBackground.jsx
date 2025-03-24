import React, { useState, useEffect } from 'react';
import CoolButton from './CoolButton.jsx';

export default function VideoBackground() {
  const [isVisible, setIsVisible] = useState(false);
  const [isHovering, setIsHovering] = useState(false);

  useEffect(() => {
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

      {/* Overlay con gradiente */}
      <div className="absolute inset-0 bg-gradient-to-b from-[#2e3a3d]/80 via-[#2e3a3d]/70 to-[#2e3a3d]/90"></div>

      {/* Contenuto sopra il video */}
      <div className="absolute inset-0 flex flex-col justify-center items-center px-4">
        <div className={`transition-all duration-1000 ${isVisible ? 'translate-y-0 opacity-100' : 'translate-y-10 opacity-0'} text-center w-full`}>
          
          {/* Container responsivo con immagini laterali */}
          <div className="flex flex-col lg:flex-row items-center justify-center gap-8 w-full">
            
            {/* Immagine sinistra */}
            <div className="w-28 h-28 lg:w-auto lg:h-48 animate-bounce-slow hidden lg:flex items-center justify-center">
              <img
                src="/lemure.png"
                alt="Left bounce"
                className="h-full w-auto drop-shadow-[0_4px_20px_rgba(2,209,255,0.4)]"
              />
            </div>

            {/* Centro (logo, testo, bottone) */}
            <div className="flex flex-col items-center justify-center text-center max-w-2xl">
              <img 
                className="h-48 w-auto mb-6 drop-shadow-[0_0_15px_rgba(0,0,0,0.5)]" 
                src="madagabot.png" 
                alt="Madagabot Logo" 
              />

              <p className="text-lg md:text-xl text-white mb-10 leading-relaxed font-medium drop-shadow-[0_2px_4px_rgba(0,0,0,0.8)] text-shadow">
                Ciao, siamo i Madagabot e abbiamo replicato la scena iconica di Re Julien,
                ma in versione <span className="text-[#02d1ff] font-bold">dark techno</span>
              </p>

              <div className="flex justify-center">
                
                <CoolButton />

              </div>
            </div>

            {/* Immagine destra */}
            <div className="w-28 h-28 lg:w-auto lg:h-48 animate-bounce-slow hidden lg:flex items-center justify-center">
              <img
                src="/lemure.png"
                alt="Right bounce"
                className="h-full w-auto drop-shadow-[0_4px_20px_rgba(2,209,255,0.4)]"
              />
            </div>
          </div>

          {/* Immagini in colonna per mobile */}
          <div className="mt-8 flex flex-col items-center gap-6 lg:hidden">
            <img src="/lemure.png" alt="Left bounce" className="w-28 h-28 animate-bounce-slow drop-shadow-[0_4px_20px_rgba(2,209,255,0.4)]" />
            <img src="/lemure.png" alt="Right bounce" className="w-28 h-28 animate-bounce-slow drop-shadow-[0_4px_20px_rgba(2,209,255,0.4)]" />
          </div>
        </div>
      </div>

      {/* Decorazioni geometriche */}
      <div className="absolute bottom-10 left-10 w-32 h-32 border border-[#02d1ff]/20 rounded-full opacity-20 animate-pulse"></div>
      <div className="absolute top-20 right-20 w-16 h-16 border border-[#02d1ff]/20 rounded-full opacity-20 animate-pulse" style={{ animationDelay: '1s' }}></div>
    </div>
  );
}
