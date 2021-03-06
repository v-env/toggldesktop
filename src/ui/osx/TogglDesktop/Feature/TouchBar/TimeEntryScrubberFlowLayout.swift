//
//  TimeEntryScrubberFlowLayout.swift
//  TogglDesktop
//
//  Created by Nghia Tran on 10/8/19.
//  Copyright © 2019 Alari. All rights reserved.
//

import Cocoa

protocol TimeEntryScrubberFlowLayoutDelegate: class {

    func timeEntryScrubberTitle(at index: Int) -> String
}

@available(OSX 10.12.2, *)
final class TimeEntryScrubberFlowLayout: NSScrubberFlowLayout {

    private struct Constants {
        static let BarItemHeight: CGFloat = 30
    }

    // MARK: Variables

    weak var delegate: TimeEntryScrubberFlowLayoutDelegate?
    private var itemAttributes: [NSScrubberLayoutAttributes] = []
    private var totalWidth: CGFloat = 0

    // MARK: Overriden

    override func prepare() {
        super.prepare()
        guard let scrubber = scrubber else { return }
        guard let numberOfItems = scrubber.dataSource?.numberOfItems(for: scrubber) else { return }

        // Prepare data
        var itemAttributes: [NSScrubberLayoutAttributes] = []
        var totalWidth: CGFloat = 0
        var x: CGFloat = 0

        // Calculate the size for each items
        for i in 0..<numberOfItems {
            guard let title = delegate?.timeEntryScrubberTitle(at: i) else { continue }
            let attribute = NSScrubberLayoutAttributes(forItemAt: i)

            // Get the size depend on the length of text
            let templateBtn = NSTextField(string: "")
            templateBtn.stringValue = title
            //let width = templateBtn.cell!.cellSize(forBounds: CGRect(x: 0, y: 0, width: 1000.0, height: Constants.BarItemHeight)).width
            let width: CGFloat = 120

            // Override the size
            var size = CGSize(width: width, height: Constants.BarItemHeight)
            size.width += 30 // Padding for button

            let frame = CGRect(x: x, y: 0, width: size.width, height: Constants.BarItemHeight)
            attribute.frame = frame

            //
            x += size.width + itemSpacing
            totalWidth += size.width + itemSpacing
            itemAttributes.append(attribute)
        }

        self.itemAttributes = itemAttributes
        self.totalWidth = totalWidth
    }

    override func layoutAttributesForItem(at index: Int) -> NSScrubberLayoutAttributes? {
        return itemAttributes[safe: index]
    }

    override func layoutAttributesForItems(in rect: NSRect) -> Set<NSScrubberLayoutAttributes> {
        let atts = itemAttributes.compactMap({ (att) -> NSScrubberLayoutAttributes? in
            if att.frame.intersects(rect) {
                return att
            }
            return nil
        })
        return Set(atts)
    }

    override var scrubberContentSize: NSSize {
        var size = super.scrubberContentSize
        size.width = totalWidth
        return size
    }
}
