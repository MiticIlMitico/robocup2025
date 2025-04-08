import React from 'react';
import Hero from '../components/Hero';

function ChiSiamo() {
  return (
    <div className="min-h-screen flex flex-col bg-transparent">
      <Hero
        title='"I like to move it, move it"'
        description="Ciao, siamo dei ragazzi dell'istituto industriale informatico e abbiamo realizzato la scena iconica di Re Julien durante la sua festa"
        imageUrl="me_and_gloria.png"
        imageAlt="Team RoboCup"
        imageRight={true}
        showButtons={true}
        withShadow={false}
        showDescription={true}
        primaryButton={{ text: "Scopri di più", href: "#" }}
        secondaryButton={{ text: "Contattaci", href: "#" }}
      />
    </div>
  );
}

export default ChiSiamo;
