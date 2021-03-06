cd /archive/website/comics/comics/conan

# Regular comic-sized books
# Conan (from Dark Horse), Conan The Barbarian, Conan Annual, 
# Marvel Feature, Giant-Sized Conan, Conan The Adventurer

grep "^#\|^cron\|^cn\|^cb\|^ca\|^mf\|^gc\|^cv\|^fg\|^te\|^bowl\|^sotd\|^bb" conan > conancomics


# Magazine-sized books
# Savage Sword, Savage Tales, Marvel Super Special, Graphic Novels

grep "^#\|^ss\|^st\|^ss\|^gn" conan > conanmags
