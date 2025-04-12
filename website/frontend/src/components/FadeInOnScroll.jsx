import React, { useEffect, useRef, useState } from 'react';

function FadeInOnScroll({ children, className = '' }) {
  const ref = useRef(null);
  const [isVisible, setVisible] = useState(false);

  useEffect(() => {
    const el = ref.current;

    // Rileva se siamo su mobile
    const isMobile = window.innerWidth <= 768;

    const observer = new IntersectionObserver(
      ([entry]) => {
        if (entry.isIntersecting) {
          setVisible(true);
          observer.unobserve(el); // Attiva una volta sola
        }
      },
      {
        threshold: isMobile ? 0.2 : 0.5, // Su mobile attiva prima (più in alto)
      }
    );

    if (el) observer.observe(el);
    return () => el && observer.unobserve(el);
  }, []);

  return (
    <div
      ref={ref}
      className={`transition-all duration-1000 ease-out transform ${
        isVisible ? 'opacity-100 translate-y-0' : 'opacity-0 translate-y-10'
      } ${className}`}
    >
      {children}
    </div>
  );
}

export default FadeInOnScroll;
