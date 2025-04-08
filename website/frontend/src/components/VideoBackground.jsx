import React, { useState, useEffect } from 'react';
import CoolButton from './CoolButton.jsx';
import OverlayLiane from './OverlayLiane';
import GitHubButton from './GitHubButton.jsx';

const scrollToProject = () => {
  const section = document.getElementById("project-section");
  if (section) {
    section.scrollIntoView({ behavior: "smooth" });
  }
};


export default function VideoBackground() {
  const [isVisible, setIsVisible] = useState(false);

  useEffect(() => {
    const timer = setTimeout(() => {
      setIsVisible(true);
    }, 300);
    return () => clearTimeout(timer);
  }, []);

  return (
    <div className="relative w-full h-screen overflow-hidden">
      {/* SVG liane sopra tutto */}
      <OverlayLiane />

      {/* Video di sfondo */}
      <video
        className="absolute top-0 left-0 w-full h-full object-cover"
        autoPlay
        loop
        muted
        playsInline
      >
        <source src="/0316_encoder.mp4" type="video/mp4" />
      </video>

      {/* Overlay con gradiente */}
      <div className="absolute inset-0 bg-[linear-gradient(to_bottom,_#000000cc_0%,_#1a1a1ab3_40%,_#1a1a1ab3_70%,_#1a1a1ab3_100%)]"></div>

      {/* Contenuto sopra il video */}
      <div className="absolute inset-0 flex flex-col justify-center items-center px-4" >
        <div
          className={`transition-all duration-1000 ${
            isVisible ? 'translate-y-0 opacity-100' : 'translate-y-10 opacity-0'
          } text-center w-full`}
        >
          {/* Contenuto centrale */}
          <div className="flex flex-col items-center justify-center text-center max-w-2xl mx-auto">
            <img
              className="h-48 w-auto mb-4 drop-shadow-[0_0_15px_rgba(0,0,0,0.5)]"
              src="madagabot.png"
              alt="Madagabot Logo"
            />

            <p className="text-lg md:text-xl text-white mb-4 leading-relaxed font-medium drop-shadow-[0_2px_4px_rgba(0,0,0,0.8)] text-shadow">
              Ciao, siamo i Madagabot e abbiamo replicato la scena iconica di Re Julien,
              ma in versione <span className="font-bold">dark techno</span>
            </p>

            {/* Bottone con immagini laterali */}
            <div className="flex items-center justify-center gap-4 mx-auto">

            <button
  onClick={scrollToProject}
  className="group relative inline-block px-6 py-[10px] text-base text-[#02d1ff] text-center tracking-wider bg-transparent border-2 border-[#02d1ff] rounded-full shadow-[inset_0_0_0_0_#02d1ff] transition-[box-shadow,color,transform] duration-500 ease-out hover:text-white hover:shadow-[inset_0_-100px_0_0_#02d1ff] active:scale-90"
>
  <span className="block transition-opacity duration-300 group-hover:opacity-0 relative z-10">
    Scopri di più
  </span>

  <span className="absolute inset-0 flex items-center justify-center opacity-0 group-hover:opacity-100 transition-opacity duration-300 text-white z-10">
    <svg
      className="w-6 h-6"
      xmlns="http://www.w3.org/2000/svg"
      fill="currentColor"
      viewBox="0 0 24 24"
    >
      <path d="M12 16.5l-6-6h12l-6 6z" />
    </svg>
  </span>
</button>


<a
  href="https://github.com/MiticIlMitico/robocup2025"
  target="_blank"
  rel="noopener noreferrer"
  className="flex items-center justify-center gap-2 px-4 py-3 text-white text-base leading-4 bg-[#181717] outline outline-[3px] outline-[#181717] outline-offset-[-3px] rounded-full border-none cursor-pointer transition-all duration-400 group hover:bg-white"
>
  <svg
    width={24}
    height={24}
    viewBox="0 0 24 24"
    fill="none"
    xmlns="http://www.w3.org/2000/svg"
  >
    <path
      className="transition-all duration-400 group-hover:fill-[#181717]"
      d="M12 0.296997C5.37 0.296997 0 5.67 0 12.297C0 17.6 3.438 22.097 8.205 23.682C8.805 23.795 9.025 23.424 9.025 23.105C9.025 22.82 9.015 22.065 9.01 21.065C5.672 21.789 4.968 19.455 4.968 19.455C4.422 18.07 3.633 17.7 3.633 17.7C2.546 16.956 3.717 16.971 3.717 16.971C4.922 17.055 5.555 18.207 5.555 18.207C6.625 20.042 8.364 19.512 9.05 19.205C9.158 18.429 9.467 17.9 9.81 17.6C7.145 17.3 4.344 16.268 4.344 11.67C4.344 10.36 4.809 9.29 5.579 8.45C5.444 8.147 5.039 6.927 5.684 5.274C5.684 5.274 6.689 4.952 8.984 6.504C9.944 6.237 10.964 6.105 11.984 6.099C13.004 6.105 14.024 6.237 14.984 6.504C17.264 4.952 18.269 5.274 18.269 5.274C18.914 6.927 18.509 8.147 18.389 8.45C19.154 9.29 19.619 10.36 19.619 11.67C19.619 16.28 16.814 17.295 14.144 17.59C14.564 17.95 14.954 18.686 14.954 19.81C14.954 21.416 14.939 22.706 14.939 23.096C14.939 23.411 15.149 23.786 15.764 23.666C20.565 22.092 24 17.592 24 12.297C24 5.67 18.627 0.296997 12 0.296997Z"
      fill="white"
    />
  </svg>
  <p className="text-white text-base transition-all duration-400 group-hover:text-[#181717]">
    GitHub
  </p>
</a>




               {/* 
              <CoolButton />
              <GitHubButton href="https://github.com/madagabot/progetto-re-julien" />
              */}
            </div>
          </div>

          {/* Immagini in colonna per mobile */}
          <div className="mt-8 flex flex-col items-center gap-6 lg:hidden">
            <img
              src="/lemure.png"
              alt="Left bounce"
              className="w-28 h-28 animate-bounce-slow drop-shadow-[0_4px_20px_rgba(2,209,255,0.4)]"
            />
            <img
              src="/lemure.png"
              alt="Right bounce"
              className="w-28 h-28 animate-bounce-slow drop-shadow-[0_4px_20px_rgba(2,209,255,0.4)]"
            />
          </div>
        </div>
      </div>

      {/* Gradiente in basso  
      <div className="absolute bottom-0 left-0 w-full h-60 bg-gradient-to-t from-[#e7fbff] to-[#e7fbff]/0 pointer-events-none z-20"></div>
      */}
    </div>
  );
}
