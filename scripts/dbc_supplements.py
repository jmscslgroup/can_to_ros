#Signals that are not (yet) in the strym DBC files but are needed by can_to_ros.
#Each supplement adds only messages whose frame ID is missing from the main DBC,
#so once strym gains a message, the strym definition wins and the supplement is ignored for it.
import os

import cantools

SUPPLEMENTS = {
    ('TOYOTA', 'RAV4'): ['dbc/toyota_rav4_supplement.dbc'],
}


def addSupplementalDBCs(db, vin_details):
    """Add supplement DBC messages for this make/model to the cantools Database db (in place)."""
    here = os.path.dirname(os.path.abspath(__file__))
    key = (vin_details.get('Make'), vin_details.get('Model'))
    existing = {m.frame_id for m in db.messages}
    for rel in SUPPLEMENTS.get(key, []):
        sup = cantools.database.load_file(os.path.join(here, rel), strict=False)
        for m in sup.messages:
            if m.frame_id in existing:
                print('supplement %s: msg %d already in main DBC, keeping main definition' % (rel, m.frame_id))
                continue
            db.messages.append(m)
            existing.add(m.frame_id)
            print('supplement %s: added msg %d (%s)' % (rel, m.frame_id, m.name))
    db.refresh()
    return db
