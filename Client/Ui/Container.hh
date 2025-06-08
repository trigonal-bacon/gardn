#pragma once

#include <Client/Ui/Element.hh>

#include <initializer_list>

namespace Ui {
    class Container : public Element {
    public:
        Container(std::initializer_list<Element *>, float = 0, float = 0, Style = {});

        virtual void on_render(Renderer &) override;

        virtual void poll_events() override;
    };

    class HContainer : public Container {
    protected:
        float outer_pad = 0;
        float inner_pad = 0;
    public:
        HContainer(std::initializer_list<Element *>, float = 0, float = 0, Style = {});

        virtual void refactor() override;
    };

    class VContainer : public Container {
    protected:
        float outer_pad = 0;
        float inner_pad = 0;
    public:
        VContainer(std::initializer_list<Element *>, float = 0, float = 0, Style = {});

        virtual void refactor() override;
    };

    class HFlexContainer final : public Container {
        float inner_pad = 0;
        float outer_pad = 0;
    public:
        HFlexContainer(Element *, Element *, float, float, Style = {});

        virtual void refactor() override;
        virtual void on_render(Renderer &) override;
    };
}