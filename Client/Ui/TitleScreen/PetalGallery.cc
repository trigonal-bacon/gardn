#include <Client/Ui/TitleScreen/TitleScreen.hh>

#include <Client/Ui/InGame/GameInfo.hh>
#include <Client/Ui/ScrollContainer.hh>
#include <Client/Ui/StaticText.hh>

#include <Client/Game.hh>
#include <Client/StaticData.hh>
#include <Client/Assets/Assets.hh>

#include <algorithm>
#include <cstring>

using namespace Ui;

GalleryPetal::GalleryPetal(PetalID::T id, float w) : 
    Element(w,w,{ .fill = 0x40000000, .round_radius = w/20 , .h_justify = Style::Left }), id(id) {}

void GalleryPetal::on_render(Renderer &ctx) {
    if (!Game::seen_petals[id]) {
        Element::on_render(ctx);
        ctx.draw_text("?", { .fill = 0xffeeeeee, .size = width / 2, .stroke_scale = 0});
    } else {
        ctx.scale(width / 60);
        draw_loadout_background(ctx, id);
    }
}

void GalleryPetal::on_event(uint8_t event) {
    if (event != kFocusLost && id != PetalID::kNone && id != PetalID::kCrown && Game::seen_petals[id]) {
        rendering_tooltip = 1;
        tooltip = Ui::UiLoadout::petal_tooltips[id];
    } else
        rendering_tooltip = 0;
}

PetalsCollectedIndicator::PetalsCollectedIndicator(float w) : Element(w,w,{}) {}

void PetalsCollectedIndicator::on_render(Renderer &ctx) {
    uint32_t colct = 0;
    uint32_t totct = 0;
    for (PetalID::T i = PetalID::kBasic; i < PetalID::kNumPetals; ++i) {
        colct += Game::seen_petals[i] != 0;
        ++totct;
    }
    ctx.set_fill(0x80000000);
    ctx.begin_path();
    ctx.arc(0,0,width/2);
    ctx.fill();
    ctx.set_fill(0xc0eeeeee);
    ctx.begin_path();
    ctx.move_to(0,0);
    ctx.partial_arc(0,0,width/2*0.8,-M_PI/2,-M_PI/2+2*M_PI*colct/totct,0);
    ctx.close_path();
    ctx.fill();
}

static Element *make_scroll() {
    Element *elt = new Ui::VContainer({}, 10, 10, {});

    PetalID::T id_list[PetalID::kNumPetals - 1]; // -1 for crown
    uint8_t count = 0;
    for (PetalID::T i = 0; i < PetalID::kNumPetals; ++i) {
        if (i != PetalID::kCrown) { 
            id_list[count++] = i;
        }
    }
    std::sort(id_list, id_list + PetalID::kNumPetals, [](PetalID::T a, PetalID::T b){
        if (a == PetalID::kNone) return true;
        if (b == PetalID::kNone) return false;
        if (PETAL_DATA[a].rarity < PETAL_DATA[b].rarity) return true;
        if (PETAL_DATA[a].rarity > PETAL_DATA[b].rarity) return false;
        return strcmp(PETAL_DATA[a].name, PETAL_DATA[b].name) <= 0;
    });
    for (PetalID::T i = PetalID::kBasic; i < PetalID::kNumPetals;) {
        Element *row = new Ui::HContainer({}, 0, 10, { .v_justify = Style::Top });
        for (uint8_t j = 0; j < 4 && i < PetalID::kNumPetals; ++j, ++i) {
            row->add_child(new GalleryPetal(id_list[i], 60));
        }
        row->refactor();
        elt->add_child(row);
    }
    return new Ui::ScrollContainer(elt, 300);
}

Element *Ui::make_petal_gallery() {
    Element *elt = new Ui::VContainer({
        new Ui::StaticText(25, "Petal Gallery"),
        make_scroll()
    }, 15, 10, { 
        .fill = 0xff5a9fdb,
        .line_width = 7,
        .round_radius = 3,
        .animate = [](Element *elt, Renderer &ctx){
            ctx.translate(0, (1 - elt->animation) * 2 * elt->height);
        },
        .should_render = [](){
            return Ui::panel_open == Panel::kPetals && Game::should_render_title_ui();
        },
        .h_justify = Style::Left,
        .v_justify = Style::Bottom
    });
    Ui::Panel::petal_gallery = elt;
    return elt;
}