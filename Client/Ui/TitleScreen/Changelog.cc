#include <Client/Ui/TitleScreen/TitleScreen.hh>

#include <Client/Ui/Container.hh>
#include <Client/Ui/Extern.hh>
#include <Client/Ui/StaticText.hh>

#include <Client/Game.hh>

using namespace Ui;

static std::unique_ptr<Element> make_divider() {
    return el<Element>(400, 6, Style{ .fill = 0x20000000, .line_width = 3, .round_radius = 3, .v_justify = Style::Top });
}

static std::unique_ptr<Element> make_date(std::string const date) {
    return el<StaticText>(20, date, Style{ .fill = 0xffffffff, .h_justify = Style::Left, .v_justify = Style::Top });
}

static std::unique_ptr<Element> make_paragraph(std::string const contents) {
    return el<HContainer>(make_children(
        el<StaticText>(14, "-", Style{ .fill = 0xffffffff, .v_justify = Style::Top }),
        el<StaticParagraph>(360, 14, contents)
    ), 0, 10, Style{ .h_justify = Style::Left, .v_justify = Style::Top });
}

static std::unique_ptr<Element> make_entries(std::initializer_list<std::string> contents) {
    auto elt = el<VContainer>(make_children(), 5, 3, Style{ .h_justify = Style::Left, .v_justify = Style::Top });
    for (std::string const str : contents)
        elt->add_child(make_paragraph(str));
    return elt;
}

static std::unique_ptr<Element> make_changelog_contents() {
    auto elt = el<VContainer>(make_children(
        make_date("October 20th 2025"),
        make_entries({
            "Added 1 new petal",
            "Peas now split into 4 orthogonal directions",
            "Even more bugfixes"
        }),
        make_divider(),
        make_date("August 30th 2025"),
        make_entries({
            "Added mobile touch controls",
            "Neutral mobs will now aggro the leader if above level 60",
            "More bugfixes"
        }),
        make_divider(),
        make_date("August 10th 2025"),
        make_entries({
            "Added TDM",
            "Made some faster mobs slower",
            "Various bugfixes"
        }),
        make_divider(),
        make_date("July 31st 2025"),
        make_entries({
            "Added 1 new petal",
            "Changed initial petal spawn delay from 2.5s to 1s"
        }),
        make_divider(),
        make_date("June 15th 2025"),
        make_entries({
            "Finished base game",
            "Added 3 new petals",
            "Drop rates are now autobalanced",
            "Spawning a petal for the first time now takes 2.5 extra seconds"
        }),
        make_divider(),
        make_date("June 11th 2025"),
        make_entries({
            "Added 1 new mob",
            "Balanced some mob stats",
            "Revamped petal descriptions"
        }),
        make_divider(),
        make_date("June 7th 2025"),
        make_entries({
            "Finished changelog",
            "Tweaked some petal stats",
            "Added settings and mob gallery"
        }),
        make_divider(),
        make_date("June 6th 2025"),
        make_entries({
            "Added Changelog",
            "Added 2 new petals",
            "Added 1 new mob",
        }),
        make_divider(),
        make_date("March 23rd 2025"),
        make_entries({
            "Game is now playable",
            "Added spawner petals"
        }),
        make_divider(),
        el<StaticText>(14, "Older changelog entries not available")
    ), 0, 10, Style{});
    return el<ScrollContainer>(std::move(elt), 300);
}

std::unique_ptr<Element> Ui::make_changelog() {
    auto elt = el<VContainer>(make_children(
        el<StaticText>(25, "Changelog"),
        make_changelog_contents()
    ), 15, 10, Style{ 
        .fill = 0xff5a9fdb,
        .line_width = 7,
        .round_radius = 3,
        .animate = [](Element *elt, Renderer &ctx){
            ctx.translate(0, (1 - elt->animation) * 2 * elt->height);
        },
        .should_render = [](){
            return Ui::panel_open == Panel::kChangelog && Game::should_render_title_ui();
        },
        .h_justify = Style::Left,
        .v_justify = Style::Bottom
    });
    Ui::Panel::changelog = elt.get();
    return elt;
}