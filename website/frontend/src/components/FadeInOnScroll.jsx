import React, { useEffect, useRef, useState } from 'react';

function FadeInOnScroll({ children, className = '' }) {
  const ref = useRef(null);
  const [isVisible, setVisible] = useState(false);

  useEffect(() => {
    const el = ref.current;
    const observer = new IntersectionObserver(
      ([entry]) => {
        if (entry.isIntersecting) {
          setVisible(true);
          observer.unobserve(el); // Mostra una volta sola
        }
      },
      { threshold: 0.5 }
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
