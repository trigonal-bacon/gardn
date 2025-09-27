#include <Client/Ui/TitleScreen/TitleScreen.hh>

#include <Client/Ui/Container.hh>
#include <Client/Ui/Extern.hh>
#include <Client/Ui/StaticText.hh>

#include <Client/Game.hh>

using namespace Ui;

static Element *make_divider() {
    return new Element(400, 6, { .fill = 0x20000000, .line_width = 3, .round_radius = 3, .v_justify = Style::Top });
}

static Element *make_date(std::string const date) {
    return new Ui::StaticText(20, date, { .fill = 0xffffffff, .h_justify = Style::Left, .v_justify = Style::Top });
}

static Element *make_paragraph(std::string const contents) {
    return new Ui::HContainer({
        new Ui::StaticText(14, "-", { .fill = 0xffffffff, .v_justify = Style::Top }),
        new Ui::StaticParagraph(360, 14, contents)
    }, 0, 10, { .h_justify = Style::Left, .v_justify = Style::Top });
}

static Element *make_entries(std::initializer_list<std::string const> contents) {
    Element *elt = new Ui::VContainer({}, 5, 3, { .h_justify = Style::Left, .v_justify = Style::Top });
    for (std::string const str : contents)
        elt->add_child(make_paragraph(str));
    return elt;
}

static Element *make_changelog_contents() {
    Element *elt = new Ui::VContainer({
        make_date("September 27th 2025"),
        make_entries({
            "Game updates are now seamless for players",
            "Multiple servers may exist at the same time"
        }),
        make_divider(),
        make_date("September 14th 2025"),
        make_entries({
            "Added more mobile buttons",
            "Fixed nickname input on mobile"
        }),
        make_divider(),
        make_date("September 11th 2025"),
        make_entries({
            "Players can now rejoin the game for up to 1 minute after leaving"
        }),
        make_divider(),
        make_date("September 3rd 2025"),
        make_entries({
            "Buffed Bone",
            "Teammates are now displayed on the minimap"
        }),
        make_divider(),
        make_date("August 30th 2025"),
        make_entries({
            "Added mobile touch controls",
            "Enabled collision between friendly entities",
            "Neutral mobs will now aggro the leader if above level 60",
            "More bugfixes"
        }),
        make_divider(),
        make_date("August 25th 2025"),
        make_entries({
            "Buffed Peas",
            "Reworked Yggdrasil",
            "Rose and Dahlia can now heal teammates (including Diggers!)"
        }),
        make_divider(),
        make_date("August 20th 2025"),
        make_entries({
            "Added chat"
        }),
        make_divider(),
        make_date("August 14th 2025"),
        make_entries({
            "Added ability to manually restore score and petals for players who leave the game without dying"
        }),
        make_divider(),
        make_date("August 10th 2025"),
        make_entries({
            "Added TDM",
            "Made some faster mobs slower",
            "Various bugfixes"
        }),
        make_divider(),
        make_date("August 4th 2025"),
        make_entries({
            "Increased max slot number to 12"
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
        new Ui::StaticText(14, "Older changelog entries not available")
    }, 0, 10, {});
    return new Ui::ScrollContainer(elt, 300);
}

Element *Ui::make_changelog() {
    Element *elt = new Ui::VContainer({
        new Ui::StaticText(25, "Changelog"),
        make_changelog_contents()
    }, 15, 10, { 
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
    Ui::Panel::changelog = elt;
    return elt;
}