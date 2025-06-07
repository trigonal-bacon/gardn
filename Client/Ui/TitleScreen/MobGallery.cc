#include <Client/Ui/TitleScreen/TitleScreen.hh>

#include <Client/Ui/Container.hh>
#include <Client/Ui/StaticText.hh>
#include <Client/Ui/Extern.hh>


#include <Client/Assets/Assets.hh>
#include <Client/Game.hh>

#include <Shared/Helpers.hh>
#include <Shared/StaticData.hh>

using namespace Ui;

GalleryMob::GalleryMob(MobID::T id, float w) : 
    Element(w,w,{ .fill=0xff5a9fdb, .stroke_hsv=1, .line_width=3, .round_radius=3, .v_justify=Style::Top }), id(id) {}

void GalleryMob::on_render(Renderer &ctx) {
    Element::on_render(ctx);
    ctx.begin_path();
    ctx.round_rect(-width / 2, -height / 2, width, height, style.round_radius);
    ctx.clip();
    struct MobData const &data = MOB_DATA[id];
    ctx.rotate(-3*M_PI/4);
    float radius = (data.radius.upper + data.radius.lower) / 2;
    if (radius > width * 0.5) ctx.scale(0.5 * width / radius);
    ctx.scale(0.5);
    draw_static_mob(id, ctx, { .radius = radius });
    if (data.attributes.segments > 1) {
        ctx.translate(-2 * radius, 0);
        draw_static_mob(id, ctx, { .radius = radius, .flags = 1<<1 });
    }
}

static Element *make_mob_drops(MobID::T id) {
    Element *elt = new Ui::HContainer({}, 0, 5, { .h_justify = Style::Left });
    struct MobData const &data = MOB_DATA[id];
    for (MobDrop const &drop : data.drops) {
        elt->add_child(new Ui::VContainer({
            new GalleryPetal(drop.id, 45),
            new StaticText(12, format_pct(drop.chance * 100))
        }, 0, 5, { .h_justify = Style::Left }));
    }
    return elt;
}

static Element *make_mob_card(MobID::T id) {
    Element *elt = new Ui::VContainer({
        new Ui::Element(300,0),
        new Ui::HFlexContainer(
            new Ui::VContainer({
                new Ui::StaticText(20, MOB_DATA[id].name, { .fill = 0xffffffff, .h_justify = Style::Left }),
                new Ui::StaticText(14, RARITY_NAMES[MOB_DATA[id].rarity], { .fill = RARITY_COLORS[MOB_DATA[id].rarity], .h_justify = Style::Left }),
                new Ui::Element(0,2),
                new Ui::StaticParagraph(220, 14, MOB_DATA[id].description, { .h_justify = Style::Left })
            }, 0, 5),
            new GalleryMob(id, 60),
            10, 10
        ),
        new Ui::Element(0,10),
        make_mob_drops(id)
    }, 10, 0, { .fill = 0x40000000, .stroke_hsv = 1, .line_width = 3, .round_radius = 6, .v_justify = Style::Top });
    return elt;
}

static Element *make_scroll() {
    Element *elt = new Ui::VContainer({}, 0, 10, {});
    for (MobID::T i = 0; i < MobID::kNumMobs; ++i) 
        elt->add_child(make_mob_card(i));

    return new Ui::ScrollContainer(elt, 300);
}

Element *Ui::make_mob_gallery() {
    Element *elt = new Ui::VContainer({
        new Ui::StaticText(25, "Mob Gallery"),
        make_scroll()
    }, 20, 10, { 
        .fill = 0xff5a9fdb,
        .line_width = 7,
        .round_radius = 3,
        .animate = [](Element *elt, Renderer &ctx){
            ctx.translate(0, (1 - elt->animation) * 2 * elt->height);
        },
        .should_render = [](){
            return Ui::panel_open == Panel::kMobs && Game::should_render_title_ui();
        },
        .h_justify = Style::Left,
        .v_justify = Style::Bottom
    });
    Ui::Panel::mob_gallery = elt;
    return elt;
}