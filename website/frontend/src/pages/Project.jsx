import React from 'react';
import Hero from '../components/Hero';
import CircuitCodeSection from '../components/CircuitCodeSection';
import DoubleFeature from '../components/DoubleFeature';
import GalleryCircuiti from '../components/GalleryCircuiti';
import FadeInOnScroll from '../components/FadeInOnScroll';

function Team() {
  return (
    <div className="min-h-screen flex flex-col bg-transparent">
      <FadeInOnScroll>
        <Hero
          title='Re Julien'
          description="Questo è il nostro Re Julien, realizzato con arduino mega, maestro pololu 12 e sensore di distanza. Riesce a far funzionare in modo sincronizzato i servomotori delle braccia e della testa insieme al movimento delle ruote"
          imageUrl="re-julien-and-hardware.png"
          imageAlt="Team RoboCup"
          imageRight={true}
          showButtons={false}
          showDescription={true}
          mainImageHeight='600px'
          withShadow={false} 
          extraSections={[
            { 
              image: "/icon1.png", 
              text: "Trono mobile realizzato interamente in polistirolo", 
              imageHeight: "60px" 
            },
            { image: "/servo.png", 
              text: "Movimento sincronizzato dei motori con i servo", 
              imageHeight: "50px",
              padding: "0.5rem",
              margin: "0.5rem 0",
            },
            { image: "/infrared-sensor.png", 
              text: "Uso del sensore di distanza per riconoscere ostacoli", 
              imageHeight: "50px",
              padding: "0.5rem",
              margin: "0.5rem 0",
            },
          ]}
        />
      </FadeInOnScroll>
      
      <FadeInOnScroll>
      <Hero
        title='Alex il Leone'
        description="Questo invece è Alex il Leone, realizzato con pico2, sensori di distanza. Riesce ad effettuare movimenti precisi e riconosce Re Julien tramite il suo sensore, il quale riconosce i riflessi"
        imageUrl="alex_il_leone.png"
        imageAlt="Team RoboCup"
        imageRight={false}
        showButtons={false}
        showDescription={true}
        mainImageHeight='600px'
        withShadow={false} 
        extraSections={[
          { 
            image: "/icon1.png", 
            text: "Interamente realizzato in polistirolo", 
            imageHeight: "60px" 
          },
          { image: "/servo.png", 
            text: "Movimento delle braccia tramite servo", 
            imageHeight: "50px",
            padding: "0.5rem",
            margin: "0.5rem 0",
          },
          { image: "/infrared-sensor.png", 
            text: "Uso del sensore di distanza per riconoscere Re Julien", 
            imageHeight: "50px",
            padding: "0.5rem",
            margin: "0.5rem 0",
          },
        ]}
      />
      </FadeInOnScroll>




<FadeInOnScroll>

        <Hero
        title='I pinguini'
        description="Questo invece è Alex il Leone, realizzato con pico2, sensori di distanza. Riesce ad effettuare movimenti precisi e riconosce Re Julien tramite il suo sensore, il quale riconosce i riflessi"
        imageUrl="pinguini.png"
        imageAlt="Team RoboCup"
        imageRight={true}
        showButtons={false}
        showDescription={true}
        mainImageHeight='600px'
        withShadow={false} 
        extraSections={[
          { 
            image: "/icon1.png", 
            text: "Interamente realizzato in polistirolo", 
            imageHeight: "60px" 
          },
          { image: "/servo.png", 
            text: "Movimento delle braccia tramite servo", 
            imageHeight: "50px",
            padding: "0.5rem",
            margin: "0.5rem 0",
          },
          { image: "/infrared-sensor.png", 
            text: "Uso del sensore di distanza per riconoscere Re Julien", 
            imageHeight: "50px",
            padding: "0.5rem",
            margin: "0.5rem 0",
          },
        ]}
      />
        </FadeInOnScroll>

      



      <FadeInOnScroll>

<DoubleFeature
  items={[
    {
      title: 'Liana con Scimmia',
      description: 'Due pinguini con giroscopio e Bluetooth che sfilano come militari.',
      imageUrl: 'liana.png',
      imageAlt: 'Pinguino Kowalski'
    },
    {
      title: 'Led con Bluetotth',
      description: 'Riconoscimento tramite sensore di colore tra i robot e reazione automatica.',
      imageUrl: 'led.png',
      imageAlt: 'Pinguino Rico'
    }
  ]}
/>
        </FadeInOnScroll>


      {/* 
      <CircuitCodeSection
        title="Il Circuito del Progetto"
        imageUrl="/circuito-re-julien.png"
        imageAlt="Schema Circuitale"
        titlePadding="pt-20"
        imagePadding="pb-10"
        imageHeight="auto"
      >
        {`// Codice di esempio
      #include <Servo.h>
      Servo braccio;

      void setup() {
        braccio.attach(9);
      }
      
      void loop() {
        braccio.write(90);
        delay(1000);
        braccio.write(0);
        delay(1000);
      }`}
      </CircuitCodeSection>
      */}
    </div>
  );
}

export default Team;