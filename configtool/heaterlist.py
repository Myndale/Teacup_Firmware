
import wx


class HeaterList(wx.ListCtrl):
  def __init__(self, parent):
    self.parent = parent
    self.currentItem = None
    wx.ListCtrl.__init__(self, parent, wx.ID_ANY, size = (165 + 4, 100),
                         style = wx.LC_REPORT | wx.LC_VIRTUAL | wx.LC_HRULES |
                         wx.LC_VRULES)

    self.valid = []
    self.heaterList = []

    self.InsertColumn(0, "Name")
    self.InsertColumn(1, "Pin")
    self.InsertColumn(2, "PWM")
    self.SetColumnWidth(0, 55)
    self.SetColumnWidth(1, 55)
    self.SetColumnWidth(2, 55)

    self.SetItemCount(0)

    self.attr2 = wx.ListItemAttr()
    self.attr2.SetBackgroundColour("light blue")
    self.attr3 = wx.ListItemAttr()
    self.attr3.SetBackgroundColour("pink")

    self.Bind(wx.EVT_LIST_ITEM_SELECTED, self.OnItemSelected)
    self.Bind(wx.EVT_LIST_ITEM_DESELECTED, self.OnItemDeselected)

  def updateList(self, heaterList):
    self.heaterList = heaterList
    self.valid = [True] * len(heaterList)
    self.currentItem = None
    self.parent.setItemSelected(None)
    i = self.GetFirstSelected()
    while i != -1:
      self.Select(i, False)
      i = self.GetFirstSelected()

    self.SetItemCount(len(heaterList))

  def setRowValidity(self, i, flag = False):
    if i < 0 or i >= len(self.heaterList):
      return

    self.valid[i] = flag
    self.Refresh()

  def setTableValidity(self, flag = False):
    for i in range(len(self.heaterList)):
      self.setRowValidity(i, flag)

  def OnItemSelected(self, event):
    self.currentItem = event.m_itemIndex
    self.parent.setItemSelected(self.currentItem)

  def OnItemDeselected(self, event):
    self.currentItem = None
    self.parent.setItemSelected(None)

  def getColumnText(self, index, col):
    item = self.GetItem(index, col)
    return item.GetText()

  def OnGetItemText(self, item, col):
    if item < 0 or item >= len(self.heaterList):
      return "Error - no heaters"

    s = self.heaterList[item]

    if col == 0:
      return s[0]
    elif col == 1:
      return s[1]
    elif col == 2:
      if s[2] == "1":
        return "True"
      else:
        return "False"

  def OnGetItemAttr(self, item):
    if not self.valid[item]:
      return self.attr3

    if item % 2 == 1:
      return self.attr2
    else:
      return None
