// GitHubButton.jsx
import React from 'react';

export default function GitHubButton({ href = 'https://github.com/', className = '' }) {
  return (
    <div className={`group relative ${className}`}>
      <a href={href} target="_blank" rel="noopener noreferrer">
        <button className="text-white">
          <svg
            strokeLinecap="round"
            strokeLinejoin="round"
            strokeWidth="2"
            stroke="currentColor"
            fill="none"
            viewBox="0 0 24 24"
            className="w-8 hover:scale-125 duration-200 hover:stroke-blue-400"
          >
            <path d="M9 19c-5 1.5-5-2.5-7-3m14 6v-3.87a3.37 3.37 0 0 0-.94-2.61c3.14-.35 6.44-1.54 6.44-7A5.44 5.44 0 0 0 20 4.77 5.07 5.07 0 0 0 19.91 1S18.73.65 16 2.48a13.38 13.38 0 0 0-7 0C6.27.65 5.09 1 5.09 1A5.07 5.07 0 0 0 5 4.77a5.44 5.44 0 0 0-1.5 3.78c0 5.42 3.3 6.61 6.44 7A3.37 3.37 0 0 0 9 18.13V22" />
          </svg>
        </button>
      </a>
      <span className="absolute -top-14 left-1/2 -translate-x-1/2 z-20 origin-left scale-0 px-3 rounded-lg border border-gray-300 bg-white py-2 text-sm font-bold shadow-md transition-all duration-300 ease-in-out group-hover:scale-100">
        GitHub
      </span>
    </div>
  );
}
