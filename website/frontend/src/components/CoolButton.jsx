import React from 'react';

export default function CoolButton() {
  const handleClick = () => {
    const section = document.getElementById('project-section');
    if (section) {
      section.scrollIntoView({ behavior: 'smooth' });
    }
  };

  return (
    <div className="relative flex flex-col items-center group">
      <button
        onClick={handleClick}
        className={`
          relative text-[20px] font-bold text-[#02d1ff]
          transition-all duration-300 ease-[cubic-bezier(0.23,1,0.32,1)]
          group-hover:scale-110 group-hover:drop-shadow-[0_0_20px_rgba(2,209,255,0.7)]
          active:scale-100
        `}
      >
        <span className="relative z-10">scopri di più</span>
        <span className="block h-[2px] w-full bg-[#02d1ff]"></span>
      </button>

      <svg
        xmlns="http://www.w3.org/2000/svg"
        className="mt-3 w-6 h-6 text-[#02d1ff] opacity-0 group-hover:opacity-100 transition-opacity duration-300 animate-bounce drop-shadow-[0_0_6px_rgba(2,209,255,0.8)]"
        fill="none"
        viewBox="0 0 24 24"
        stroke="currentColor"
        strokeWidth={2}
      >
        <path strokeLinecap="round" strokeLinejoin="round" d="M19 9l-7 7-7-7" />
      </svg>
    </div>
  );
}
