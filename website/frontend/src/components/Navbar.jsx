import React, { useState, useEffect } from 'react';

const Navbar = () => {
  const [scrolled, setScrolled] = useState(false);
  const [isVisible, setIsVisible] = useState(false); // Stato per la visibilità in base allo scroll

  useEffect(() => {
    const handleScroll = () => {
      const scrollY = window.scrollY;
      
      // Imposta la visibilità dopo un tot di scroll (200px)
      if (scrollY > 700 && !isVisible) {
        setIsVisible(true);
      } else if (scrollY <= 700 && isVisible) {
        setIsVisible(false);
      }

      // Cambia colore quando si supera un certo punto (50px)
      setScrolled(scrollY > 50);
    };

    window.addEventListener('scroll', handleScroll);
    return () => {
      window.removeEventListener('scroll', handleScroll);
    };
  }, [isVisible]);

  return (
    <nav className={`fixed w-full z-10 transition-all duration-500 ${
      isVisible ? 'opacity-100 translate-y-0' : 'opacity-0 -translate-y-full'
    } ${scrolled ? 'bg-[#02d1ff] shadow-md' : 'bg-transparent'}`}>
      <div className="mx-auto px-4 sm:px-6 lg:px-40">
        <div className="flex justify-between h-20 items-center">
          {/* Logo */}
          <div className="flex-shrink-0 flex items-center">
            <img className="block h-16 w-auto" src="madagabot.png" alt="RoboCup Team Logo" />
          </div>

          {/* Navigation Links */}
          <div className="hidden lg:flex lg:space-x-10">
            {['Home', 'Progetto', 'Team', 'RoboCup', 'Contatti'].map((item) => (
              <a key={item} href="#" className="text-gray-900 hover:opacity-75 inline-flex items-center px-1 pt-1 text-base font-medium">
                {item}
              </a>
            ))}
          </div>
        </div>
      </div>
    </nav>
  );
};

export default Navbar;
