import React from 'react';
import Navbar from '../components/Navbar';
import Hero from '../components/Hero';

function Team() {
  return (
    <div
      className="min-h-screen flex items-center justify-center bg-cover bg-center bg-no-repeat"
      style={{ backgroundImage: "url('/tutti5.png')" }}
    >
      <h2
  className="text-4xl md:text-5xl lg:text-9xl font-serif italic text-white text-center"
  style={{
    textShadow: `
      0 0 10px white,
      0 0 20px #ccf5ff,
      0 0 40px #ccf5ff,
      0 0 60px black
    `
  }}
>
  “Our Team”
</h2>


    </div>
  );
}

export default Team;
