#pragma once

#include <Client/Ui/Element.hh>

#include <initializer_list>
#include <vector>

namespace Ui {
    class Container : public Element {
    public:
        float outer_pad = 0;
        float inner_pad = 0;
        std::vector<Element *> children;

        Container(std::initializer_list<Element *>, float = 0, float = 0, Style = {});

        void add_child(Element *);

        virtual void on_render(Renderer &) override;
        virtual void on_render_skip(Renderer &) override;
        virtual void refactor() override;
        virtual void poll_events() override;
    };

    class HContainer : public Container {
    public:
        HContainer(std::initializer_list<Element *>, float = 0, float = 0, Style = {});

        virtual void refactor() override;
    };

    class VContainer : public Container {
    public:
        VContainer(std::initializer_list<Element *>, float = 0, float = 0, Style = {});

        virtual void refactor() override;
    };
}