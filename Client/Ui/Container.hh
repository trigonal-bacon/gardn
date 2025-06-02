#pragma once

#include <Client/Ui/Element.hh>

#include <initializer_list>

namespace Ui {
    class Container : public Element {
    public:
        Container(std::initializer_list<Element *>, float = 0, float = 0, Style = {});

        virtual void on_render(Renderer &) override;
    };

    class HContainer : public Container {
    public:
        float outer_pad = 0;
        float inner_pad = 0;
        HContainer(std::initializer_list<Element *>, float = 0, float = 0, Style = {});

        virtual void refactor() override;
    };

    class VContainer : public Container {
    public:
        float outer_pad = 0;
        float inner_pad = 0;
        VContainer(std::initializer_list<Element *>, float = 0, float = 0, Style = {});

        virtual void refactor() override;
    };

    class HFlexContainer : public Container {
    public:
        float inner_pad;
        HFlexContainer(Element *, Element *, float, Style = {});

        virtual void refactor() override;
    };
}